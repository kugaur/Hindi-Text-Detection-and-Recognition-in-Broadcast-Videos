// Final Code for text Detection and Recognition



// Include Standard C/C++ Header Files Here
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// Include OpenCV Header Files Here
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
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
#include "../../CVDRIK/CVDRIKTextDetection/CVDRIKProfileBased.h"
#include "../../CVDRIK/CVDRIKMultiChannelImage/CVDRIKMultiChannelImage.h"
#include "../../CVDRIK/CVDRIKImageProcessing/CVDRIKImageProcessing.h"
#include "../../CVDRIK/CVDRIKTextDetection/CVDRIKTextDetection.h"

int main(int argc,char** argv)
{
	//CvSVM SVM;
	//SVM.load("telugusvm_model");
	//int c     = SVM.get_support_vector_count();
	//printf("\n%d\n",c);
	
	
	
	CvCapture *capture;
	capture  = cvCreateFileCapture( argv[1]);
	if (capture == NULL)
	{
		// Continue to Next video
		printf( "\n\n Can not read video  \n\n" );
		return( 1 );
	}
	CvSize size;
	
	//determine the video size
	size = cvGetSize( cvQueryFrame( capture ) );
    int totalFrames = 0;
	totalFrames=cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	// printf( "\n\n*************************************" );
	//printf( "\n\n video name %s\ntotal frames %d\n Size %dX%d \n",argv[1],totalFrames,size.width,size.height);
	cvGrabFrame(capture);//incorporate the check on reading image check output
	
	//creating temporary images for handling the frames
	// Allocating Memory for Connected Component Analysis Objects
	int width = size.width;
	int height = size.height;
	CvMemStorage* storage = cvCreateMemStorage( 0 );
	CvSeq* contour = 0;
	int maxBlobNum = 256;
	CvRect* fgRectArray = NULL;
	fgRectArray = (CvRect*) malloc( maxBlobNum * sizeof( CvRect ) );
	float* blobSizeArray = NULL;
	blobSizeArray = (float*) malloc( maxBlobNum * sizeof( float ) );
	int minBlobSize = 0;
	// Allocate Memory for Label Image to use in CCA
	IplImage* labelImg = NULL;
	labelImg = cvCreateImage( cvSize( width , height ) , IPL_DEPTH_8U , 1 );
	
	// Allocate Memory for Color Image after labeling
	IplImage* colorImg = NULL;
	colorImg = cvCreateImage( cvSize( width , height ) , IPL_DEPTH_8U , 3 );
	IplImage* inImg = NULL;
	inImg = cvCreateImage( size , IPL_DEPTH_8U , 3 );
	IplImage* HgrayImg = NULL;
	HgrayImg = cvCreateImage( size , IPL_DEPTH_8U , 1 );
	IplImage* ErodImg = NULL;
	ErodImg = cvCreateImage( size , IPL_DEPTH_8U , 1 );
	IplImage* DilImg = NULL;
	DilImg = cvCreateImage( size , IPL_DEPTH_8U , 1 );
	IplImage* OpenImg = NULL;
	OpenImg = cvCreateImage( size , IPL_DEPTH_8U , 1 );
	int frameCount = 1;
	char sysStr[150] ; 
	sprintf( sysStr , "mkdir videoframe" );
	system( sysStr ); 
	sprintf(sysStr, "mkdir Box");
	system(sysStr);
	sprintf(sysStr, "mkdir final1");
	system(sysStr);
	sprintf(sysStr,"mkdir line");
	system(sysStr);
	sprintf(sysStr,"mkdir word");
	system(sysStr);
	sprintf(sysStr,"mkdir OCRImage");
	system(sysStr);
	int counter=1,counter1=0;
	float thresh = 0.13;
	float thresh1 = 0.13;
	// 20*10
	while(1<2)
	{
		
		inImg = cvQueryFrame( capture );//destroy image in loop
		if( !inImg ) 
		break;
		//Correlation for temporal 
		//if(computeCorrelation(inImg))
		if((frameCount-1) % 80==0)
		{

        char out[150],out1[150];
        // Canny
 		cvCanny( inImg , HgrayImg ,175, 320 , 3 );
 		//cvShowImage("Canny Video1",HgrayImg);
 		//cvWaitKey(1);
 		
 		// Element for dilation
 		IplConvKernel* element=NULL;
 		element = cvCreateStructuringElementEx(9,2,4,0,CV_SHAPE_RECT,NULL);
 		cvDilate(HgrayImg,DilImg,element);
 		//cvShowImage("Dilated Video2",DilImg);
 		//cvWaitKey(1);
 		
 		//Element for opening
 		IplConvKernel* element1=NULL;
 		element1 = cvCreateStructuringElementEx(23,9,12,4,CV_SHAPE_CROSS,NULL);
 		cvErode(DilImg,ErodImg,element1);
 		cvDilate(ErodImg,OpenImg,element1);
        //cvSaveImage(out1,OpenImg,param);
        //cvShowImage("Opened Video3",OpenImg);
        //cvWaitKey(1);
        
        int fgNum = CCA( OpenImg , labelImg , fgRectArray , blobSizeArray , minBlobSize , storage , contour );
        
  
        
        IplImage** boxImg;
		boxImg = new IplImage*[100];
        int *orgNum = (int*)malloc(sizeof(int));
        CvRect *BBox;
        BBox = (CvRect*)malloc( fgNum * sizeof( CvRect ));
        BBox = ComputeRectangularImages(inImg,boxImg, fgRectArray,fgNum,orgNum);		
		int* numWord = (int*)malloc(sizeof(int));
		IplImage** wordImg;
		wordImg = new IplImage*[40];
		int* numLine = (int*)malloc(sizeof(int));	
		IplImage** lineImg;
		lineImg = new IplImage*[10];
		
		int index = (*orgNum)*3,index2 = (*orgNum)*40,index1=0;
		CvRect *lineBox;
		lineBox = (CvRect*)malloc( index * sizeof( CvRect ));
		CvRect *finalBox;
		finalBox = (CvRect*)malloc(index2*sizeof(CvRect));
		IplImage **outImg;
		outImg = new IplImage*[index2];
		counter1=0;
		IplImage *clone;
		clone = cvCloneImage(inImg);
		for(int i=0;i< *orgNum;i++)
			cvRectangle(clone , cvPoint(BBox[ i ].x,BBox[ i ].y), cvPoint(BBox[ i ].x + BBox[ i ].width,BBox[ i ].y +BBox[ i ].height),cvScalar( 255 , 20 , 250 ) , 2);
		//cvShowImage("BoxedImage",clone);
		//cvWaitKey(20);
		sprintf(out,"Box/out_%05d.png",frameCount);		
		int param[3];
		param[0] = CV_IMWRITE_PNG_COMPRESSION ;
		param[1] = 0;
		param[2] = 0;
		cvSaveImage(out,clone,param);			
		
		for(int j=1; j<= *orgNum;j++)
		{

			IplImage* grayImg = cvCreateImage(cvGetSize(boxImg[j-1]),IPL_DEPTH_8U,1); 			
			cvCvtColor ( boxImg[j-1], grayImg, CV_RGB2GRAY );
			//cvShowImage("box",boxImg[j-1]);
			//cvWaitKey(20);
					
		    unsigned char erc = computeLineSegmentation( grayImg , numLine, lineImg);
			if( erc != 0 )
			{
				printf( "\n\n Error Code = %d \n\n" , erc );
				getchar();
			}				
			//printf("Number of lines: %d \n", *numLine);
			int yheight=0;
			for(int i =1; i<=(*numLine) ; i++)
			{
				//cvShowImage("lines",lineImg[i]);
				//cvWaitKey(20);
				//Saving Image
				IplImage* binImg = cvCreateImage(cvGetSize(lineImg[i]),IPL_DEPTH_8U,1);
				IplImage* edgeImg = cvCreateImage(cvGetSize(lineImg[i]),IPL_DEPTH_8U,1);
				cvCanny(lineImg[i], edgeImg, 175, 320,3);
				cvThreshold(edgeImg, binImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
				float val = 1.0*(computeCount(binImg))/((binImg->width)*(binImg->height));
				yheight = yheight + lineImg[i]->height; 
				if(val > thresh1)// thresh1 =0.13
				{
					lineBox[index1].x = BBox[j-1].x;
					lineBox[index1].y = BBox[j-1].y + yheight - lineImg[i]->height;
					lineBox[index1].width = lineImg[i]->width;
					lineBox[index1].height = lineImg[i]->height;
					index1++;
					//cvShowImage("line",lineImg[i]);
					//cvWaitKey(20);
					//getchar();
					erc = computeWordSegmentation( lineImg[i] , numWord, wordImg);
					if( erc != 0 )
					{
						printf( "\n\n Error Code = %d \n\n" , erc );
						getchar();
					}
					//printf("Number of words : %d\n", *numWord);
					int xwidth =0;
					for(int k=1; k<= (*numWord) ;k++)
					{
					   //  cvShowImage("words",wordImg[k]);
					    // cvWaitKey(30);
					     //getchar();
						 binImg = cvCreateImage(cvGetSize(wordImg[k]),IPL_DEPTH_8U,1);
						 edgeImg = cvCreateImage(cvGetSize(wordImg[k]),IPL_DEPTH_8U,1);
						cvCanny(wordImg[k], edgeImg, 175, 320,3);
						cvThreshold(edgeImg, binImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
						float val = 1.0*(computeCount(binImg))/((binImg->width)*(binImg->height));
						//Headline detection
						xwidth = xwidth + wordImg[k]->width;
						//printf("%f\n",val);					
						if( val > thresh  and wordImg[k]->width >= 20 and wordImg[k]->height >=10)//thresh =0.13
						{
							// break image into 20*10 windows
							//final array
						
							finalBox[counter1].x = lineBox[index1-1].x + xwidth - wordImg[k]->width;
							finalBox[counter1].y = lineBox[index1 -1].y;
							finalBox[counter1].width = wordImg[k]->width;  
							finalBox[counter1].height = wordImg[k]->height;
							
						
							outImg[counter1] = cvCreateImage(cvSize(wordImg[k]->width,wordImg[k]->height),IPL_DEPTH_8U,1);
							int baseIndx = - outImg[counter1]->widthStep;
						
							for(int h1=0;h1< outImg[counter1]->height;h1++)
							{
								baseIndx  = baseIndx + outImg[counter1]->widthStep;
								 
								int currIndx = baseIndx - 1;
								
								for(int h2 =0;h2< outImg[counter1]->width;h2++)
								{
									currIndx =  currIndx + 1;
									outImg[counter1]->imageData[currIndx] = (unsigned char) wordImg[k]->imageData[currIndx];
								}
								
							}
						    
						    
							sprintf(out,"word/word_%05d.png",counter1);		
							//int param[3];
							param[0] = CV_IMWRITE_PNG_COMPRESSION ;
							param[1] = 0;
							param[2] = 0;
							cvSaveImage(out,outImg[counter1],param);
						    counter1++;
					}
			    }
				}
			}
		
		}
		IplImage *clone1,*clone2;
		clone1 = cvCloneImage(inImg);
		clone2 = cvCloneImage(inImg);
		for(int i=0;i< counter1;i++)
			cvRectangle(clone1 , cvPoint(finalBox[ i ].x,finalBox[ i ].y), cvPoint(finalBox[ i ].x + finalBox[ i ].width,finalBox[ i ].y +finalBox[ i ].height),cvScalar( 255 , 20 , 250 ) , 2);
		for(int i=0;i< index1;i++)
			cvRectangle(clone2 , cvPoint(lineBox[ i ].x,lineBox[ i ].y), cvPoint(lineBox[ i ].x + lineBox[ i ].width,lineBox[ i ].y +lineBox[ i ].height),cvScalar( 255 , 20 , 250 ) , 2);

		//cvShowImage("final Boxed",clone2);
		//cvWaitKey(20);
							sprintf(out,"final1/out_%05d.png",frameCount);		
							//int param[3];
							param[0] = CV_IMWRITE_PNG_COMPRESSION ;
							param[1] = 0;
							param[2] = 0;
							cvSaveImage(out,clone1,param);
							sprintf(out,"line/line_%05d.png",frameCount);
							cvSaveImage(out,clone2,param);		
		//testing 
	    IplImage* grayImg = cvCreateImage(cvSize(inImg->width,inImg->height),IPL_DEPTH_8U,1);
	    IplImage* beforeImg = cvCreateImage(cvSize(inImg->width,inImg->height),IPL_DEPTH_8U,1);
	    cvCvtColor ( inImg, grayImg, CV_RGB2GRAY );
	    cvZero(beforeImg);
	    for(int i=0;i<counter1;i++)
	    {
			int baseIndx = -beforeImg->widthStep + (finalBox[i].y)*(beforeImg->widthStep);
			int baseIndx1 = -outImg[i]->widthStep;
			for(int m1 = 0;m1<outImg[i]->height;m1++)
			{
				baseIndx = baseIndx + beforeImg->widthStep;
				baseIndx1 = baseIndx1 + outImg[i]->widthStep;

				int currIndx = baseIndx - 1   + finalBox[i].x ;
				int currIndx1 = baseIndx1 -1;
				for(int m2 =0; m2< outImg[i]->width ; m2++)
				{
					currIndx++;
					currIndx1++;
					beforeImg->imageData[currIndx] = (unsigned char) outImg[i]->imageData[currIndx1];
				}
			}
	    }
	    
	    //cvShowImage("gray",grayImg);
	    //cvWaitKey(20);
	    cvThreshold(beforeImg, beforeImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

	    
	    cvShowImage("before",beforeImg);
	    cvWaitKey(20);
	    
	    IplImage* OCRImg = cvCreateImage(cvSize(inImg->width, inImg->height),IPL_DEPTH_8U,1);
	    OCRImg = computeRecog(inImg, finalBox,outImg,counter1);
	    
	    cvShowImage("final",OCRImg);
	    cvWaitKey(20);
	    //getchar();
	    //getchar();
	    printf("%d\n",frameCount);
	    //Send to OCR
		sprintf(out,"OCRImage/ocr_%d.png",frameCount);		
		//int param[3];
		param[0] = CV_IMWRITE_PNG_COMPRESSION ;
		param[1] = 0;
		param[2] = 0;
		cvSaveImage(out,OCRImg,param);
	    
	    //char sysStr[150] ; 
		//sprintf( sysStr , "tesseract %s out-%d -l hin",out,frameCount );
		//system( sysStr ); 
	    //	    
		}	
        frameCount++;
	    }
        
       
	return 0;
}
