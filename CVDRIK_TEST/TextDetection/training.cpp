// code for Only text recognition
// Include Standard C/C++ Header Files Here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <vector>
// Include OpenCV Header Files Here
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <gsl/gsl_matrix_float.h>
#include <gsl/gsl_histogram.h>

// Include CVDRIK Header Files Here
#include "../../CVDRIK/CVDRIKMisc/CVDRIKMisc.h"
#include "../../CVDRIK/CVDRIKTextRecognition/CVDRIKTextRecognition.h"
#include "../../CVDRIK/CVDRIKMultiChannelImage/CVDRIKMultiChannelImage.h"
#include "../../CVDRIK/CVDRIKImageProcessing/CVDRIKImageProcessing.h"
#include "../../CVDRIK/CVDRIKTextDetection/CVDRIKTextDetection.h"

#define PI 3.141593
//comment for training with default params and uncomment for optimal parameter training
#define AUTO_TRAIN_SVM

int main( int argc , char** argv )
{
	
	// Check Usage
	if( argc != 1 )
	{
		// Show Purpose
		printf( "\n\n Purpose : text recognition in images \n\n" );
		
		// Show Usage
		printf( "\n\n Usage As : recog [ImageFileName] \n\n" );
		
		// Show Sample Command Line
		printf( "\n\n Sample Command Line : ./recog imagename.png  \n\n" );
		
		return( 0 );
	}
	float **features,*stat,*aux,*a;	
	stat= new float[5];
	aux = new float[60];
	a = new float[32];
	features = new float*[1000];
	for(int j=0;j<1000;j++)
		{
			features[j] = new float[256];  // 48 + 5 + 12 + 32
		}
	int flag=0;
	int t= 0;
	int nt = 0;
	int i=0;
	float label[1000];// 16944 + 11096 = 28040
    while(false)
    {
		char out[1000];
		if(flag==0 or nt >=500)
			{//16,944 items
				sprintf(out,"/home/gaurav/CVDRIK1/CVDRIK_TEST/TextDetection/teltextnew/text_%05d.png",t++);
				flag=1;
				label[i]=1.0;
				printf("%f\n", label[i]);
			}
		else
			{//11096 items
				sprintf(out,"/home/gaurav/CVDRIK1/CVDRIK_TEST/TextDetection/telnontextnew/nontext_%05d.png",nt++);
				flag=0;
				label[i] = 0.0;
				printf("%f\n",label[i]);
			}
			//printf("%d\n",i);
			
		IplImage* destImg = cvLoadImage(out,CV_LOAD_IMAGE_UNCHANGED);
		if(!destImg)
		  break;		
		IplImage* src = cvCreateImage(cvGetSize(destImg), IPL_DEPTH_32F, 1);
		cvConvert(destImg, src);
		IplImage* dst = cvCreateImage(cvGetSize(destImg), IPL_DEPTH_32F, 1);

		cvDCT(src, dst, 0);
		int j=0;
		int baseindex = -dst->widthStep;
		for(int y=0;y< dst->height;y++)
		{
			baseindex = baseindex + dst->widthStep;
			int currindx = baseindex -1;
			for(int x=0;x< dst->width;x++)
			{
				currindx++;
				features[i][j++] = dst->imageData[currindx];
			}
			
		}
		
		/*
		//aux = computeHOG(destImg);//HOG features
		//a = computeELBP(destImg); //ELBP features
		//stat = computeStatistical(destImg);// stat features
		//printf("%f ",features[i][50]);
		/*for(int i1=0;i1<48;i1++)
			{
				features[i][i1] = aux[i1];
			}
		for(int i2=0;i2<12;i2++)
		{
			if(aux[48 + i2] ==0 )
				features[i][48 + i2] =0;
			else
				features[i][48 + i2] = (features[i][i2] + features[i][i2 + 12] + features[i][i2 + 24] + features[i][i2 + 36])/aux[48 + i2];			
		}
		
		for(int k=0;k<5;k++)
		{  // printf("%f %f\n",stat[k], features[i][48+k]);
			features[i][60+k] = stat[k]; 
		}*/
		/*for(int k1=0;k1<32;k1++)
		{
			
			features[i][k1] = a[k1];
			
		}
			*/
			
			
			
		i++;	
			
	}
/*
   printf("\n Wrting on file\n");	
   FILE *fp;
   fp =  fopen("data.txt","w");
   for(int i=0;i<1000;i++)
   {
	   for(int j=0;j<256;j++)
	    {
		   fprintf(fp,"%f	",features[i][j]);
		}
		fprintf(fp,"\n");

	}
    fclose(fp);
    
    //SVM training
    cv::Mat trainingDataMat(1000,256,CV_32FC1,features);
    cv::Mat labelsMat(1000,1,CV_32FC1,label);
    
    //Set up SVm's parameters
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    params.degree = 0;
    params.gamma = 3;
    params.coef0 = 0;
    params.C =1000000;
    params.nu =0;
    params.p=0;
    params.class_weights = 0;
    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,1000,FLT_EPSILON);
    
    printf("\nTraining SVM\n");
    //Train SVM
    CvSVM SVM;
    //SVM.train(trainingDataMat,labelsMat,cv::Mat(),cv::Mat(),params);
    //SVM.save("telugusvm_model"); // saving
    #ifdef AUTO_TRAIN_SVM
	      printf("Finding optimal parameters to use...\n");
	
	      // Use auto-training parameter grid search (ignore params manually
	      // specified above)
	      SVM.train_auto(trainingDataMat,labelsMat,cv::Mat(),cv::Mat(),params,5);
	      params = SVM.get_params();
	
	      printf("The optimal parameters are: degree = %f, gamma = %f, coef0 = %f, C = %f, nu = %f, p = %f\n",
	        params.degree, params.gamma, params.coef0, params.C, params.nu, params.p);
          SVM.save("telsvm_model");	
	#else
	      printf("Using default parameters...\n");
	      // Use regular training with parameters manually specified above
	      SVM.train(trainingDataMat,labelsMat,cv::Mat(),cv::Mat(),params);
	      SVM.save("telsvm_model");
	#endif
  */
    
    CvSVM SVM;
    SVM.load("telsvm_model"); // loading
    //Testing 
      //FILE *ft;
      //ft = fopen("test.txt","w");
    //Preparing data
    	char out[1000];
		//sprintf(out,"/home/gaurav/CVDRIK1/CVDRIK_TEST/TextDetection/testBox/out_%d.png",1);
		//IplImage* im_gray = cvLoadImage(out,CV_LOAD_IMAGE_GRAYSCALE);
        float testdata[256];
        int k=0;
        int one=0,zero=0;
        while(1<2)
        {
		sprintf(out,"/home/gaurav/CVDRIK1/CVDRIK_TEST/TextDetection/teltextnew/text_%05d.png",k);
		IplImage* im_gray = cvLoadImage(out,CV_LOAD_IMAGE_GRAYSCALE);
		if( !im_gray ) 
		break;			
		IplImage* src = cvCreateImage(cvGetSize(im_gray), IPL_DEPTH_32F, 1);
		cvConvert(im_gray, src);
		IplImage* dst = cvCreateImage(cvGetSize(im_gray), IPL_DEPTH_32F, 1);

		cvDCT(src, dst, 0);
		int j=0;
		int baseindex = -dst->widthStep;
		for(int y=0;y< dst->height;y++)
		{
			baseindex = baseindex + dst->widthStep;
			int currindx = baseindex -1;
			for(int x=0;x< dst->width;x++)
			{
				currindx++;
				testdata[j++] = dst->imageData[currindx];
			}
			
		}
		
		//aux = computeHOG(im_gray);
		//a = computeELBP(im_gray); //ELBP features
		//stat = computeStatistical(im_gray);
		
		/*
		//printf("%f ",features[i][50]);
		for(int i1=0;i1<48;i1++)
			{
				testdata[i1] = aux[i1];
			}
		
				for(int i2=0;i2<12;i2++)
		{
			if(aux[48 + i2] ==0 )
				testdata[48 + i2] =0;
			else
				testdata[48 + i2] = (testdata[i2] + testdata[i2 + 12] + testdata[i2 + 24] + testdata[i2 + 36])/aux[48 + i2];			
		}
		for(int k1=0;k1<5;k1++)
		{  // printf("%f %f\n",stat[k], features[i][48+k]);
			testdata[60+k1] = stat[k1]; 
		}*/
		for(int k1=0;k1<256;k1++)
		{
			
			testdata[k1 ] = a[k1];
		//	printf("%f ",testdata[k1]);
			
		}
		//printf("\n");
		//Predicting
		cv::Mat testMat(256,1,CV_32FC1,testdata);
		float response = SVM.predict(testMat);
		if(response >0)
		  one++;
		  else
		  zero++;
		//printf("%f\n",response);				
		k++;
		//printf("%d\n",k);
		}
      printf("%d %d\n",one,zero);
     //fclose(ft);
 return 0;
}

