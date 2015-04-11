// code for Only text detection in a video
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
	sprintf( sysStr , "mkdir video1" );
	system( sysStr ); 
	sprintf(sysStr, "mkdir Opened");
	system(sysStr);
	sprintf(sysStr, "mkdir hBox");
	system(sysStr);
	sprintf(sysStr, "mkdir Box");
	system(sysStr);
	int counter=1;
	int counter1 =1;
	// 20*10
	while(1<2)
	{
		
		inImg = cvQueryFrame( capture );//destroy image in loop
		if( !inImg ) 
		break;
		if((frameCount-1) % 80==0)
		{
		//open file to store cvrect for frameCount
		//cvShowImage("Initial Video",inImg);
        //cvWaitKey(10);
        char out[150],out1[150];
        //sprintf(out1,"Opened/open_%d.png",(frameCount));
        sprintf(out,"video1/out_%d.png",(frameCount ));
        int param[3];
 		param[0] = CV_IMWRITE_PNG_COMPRESSION ;
 		param[1] = 0;
 		param[2] = 0;
 		cvSaveImage(out,inImg,param);
 		cvCanny( inImg , HgrayImg ,175, 320 , 3 );
 		//cvShowImage("Canny Video1",HgrayImg);
 		//cvWaitKey(1);
 		IplConvKernel* element=NULL;
 		element = cvCreateStructuringElementEx(9,2,4,0,CV_SHAPE_RECT,NULL);
 		cvDilate(HgrayImg,DilImg,element);
 		//cvShowImage("Dilated Video2",DilImg);
 		//cvWaitKey(1);
 		IplConvKernel* element1=NULL;
 		element1 = cvCreateStructuringElementEx(23,9,12,4,CV_SHAPE_CROSS,NULL);
 		cvErode(DilImg,ErodImg,element1);
 		cvDilate(ErodImg,OpenImg,element1);
        //cvSaveImage(out1,OpenImg,param);
        //cvShowImage("Opened Video3",OpenImg);
        //cvWaitKey(1);
        int fgNum = CCA( OpenImg , labelImg , fgRectArray , blobSizeArray , minBlobSize , storage , contour );
        
  
        //cvShowImage("out14",inImg);
        IplImage** boxImg;
		boxImg = new IplImage*[100];
        int *orgNum = (int*)malloc(sizeof(int));;
        CvRect *BBox;
        BBox = (CvRect*)malloc( fgNum * sizeof( CvRect ));
        BBox = ComputeRectangularImages(inImg,boxImg, fgRectArray,fgNum,orgNum);
		//if(fgNum == *orgNum)
		printf(" %d %d godly\n",fgNum, *orgNum);
		int* numWord = (int*)malloc(sizeof(int));
		IplImage** wordImg;
		wordImg = new IplImage*[40];
		int* numLine = (int*)malloc(sizeof(int));	
		IplImage** lineImg;
		lineImg = new IplImage*[10];
		float thresh = 0.15;
		for(int j=1; j<= *orgNum;j++)
		{
			//cvShowImage("boxes",boxImg[j-1]);
			//cvWaitKey(40);
			// Break text boxes into words
			//Canny 
			IplImage* grayImg = cvCreateImage(cvGetSize(boxImg[j-1]),IPL_DEPTH_8U,1); 			
			cvCvtColor ( boxImg[j-1], grayImg, CV_RGB2GRAY );
			//IplImage* binImg = cvCreateImage(cvGetSize(grayImg),IPL_DEPTH_8U,1);
			//IplImage* edgeImg = cvCreateImage(cvGetSize(grayImg),IPL_DEPTH_8U,1);
			//cvCanny(grayImg, edgeImg, 175, 320,3);
			//cvThreshold(edgeImg, binImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
						
		    unsigned char erc = computeLineSegmentation( grayImg , numLine, lineImg);
			if( erc != 0 )
			{
				printf( "\n\n Error Code = %d \n\n" , erc );
				getchar();
			}				
			//printf("Number of lines: %d \n", *numLine);
			for(int i =1; i<=(*numLine) ; i++)
			{
				//cvShowImage("lines",lineImg[i]);
				//cvWaitKey(20);
				//Saving Image
				char out[150];
				//sprintf(out,"newBox/out_%d.png",counter++);		
				//int param[3];
			//	param[0] = CV_IMWRITE_PNG_COMPRESSION ;
				//param[1] = 0;
				//param[2] = 0;
				//cvSaveImage(out,lineImg[i],param);
				erc = computeWordSegmentation( lineImg[i] , numWord, wordImg);
				if( erc != 0 )
				{
					printf( "\n\n Error Code = %d \n\n" , erc );
					getchar();
				}
				//printf("Number of words : %d\n", *numWord);
				for(int k=1; k<= (*numWord) ;k++)
				{
					
					sprintf(out,"Box/out_%d.png",counter1++);
					cvSaveImage(out,wordImg[k],param);					
					IplImage* binImg = cvCreateImage(cvGetSize(wordImg[k]),IPL_DEPTH_8U,1);
					IplImage* edgeImg = cvCreateImage(cvGetSize(wordImg[k]),IPL_DEPTH_8U,1);
					cvCanny(wordImg[k], edgeImg, 175, 320,3);
					cvThreshold(edgeImg, binImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
					float val = 1.0*(computeCount(binImg))/((binImg->width)*(binImg->height));
					//Headline detection
					
					//printf("%f\n",val);					
					if( val>thresh )
					{
						
						// break image into 20*10 windows
						int k2 = (wordImg[k]->width)/20;
						IplImage* finalImg = cvCreateImage(cvSize(20*(k2+1),10),IPL_DEPTH_8U,1);
						cvResize(wordImg[k],finalImg);

						for(int k3=0;k3<=k2;k3++)
						{
						    IplImage* outImg = cvCreateImage(cvSize(20,10),IPL_DEPTH_8U,1);
						    int baseIndx = - outImg->widthStep;
						    int baseIndx1 = -finalImg->widthStep;
						    for(int h1=0;h1< outImg->height;h1++)
						    {
								baseIndx  = baseIndx + outImg->widthStep;
								baseIndx1 = baseIndx1 + finalImg->widthStep; 
								int currIndx = baseIndx - 1;
								int currIndx1 = baseIndx1 -1;
								for(int h2 =0;h2< outImg->width;h2++)
								{
									currIndx =  currIndx + 1;
									currIndx1 = currIndx1 + 1; 
									outImg->imageData[currIndx] = finalImg->imageData[currIndx1 + 20*k3];
								}
								
							}
						    
						    
							sprintf(out,"hBox/out_%d.png",counter++);		
							int param[3];
							param[0] = CV_IMWRITE_PNG_COMPRESSION ;
							param[1] = 0;
							param[2] = 0;
						    //cvThreshold(outImg, outImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
							cvSaveImage(out,outImg,param);
						}
					}
			    }
			}
		}
		IplImage* clone;
		clone =cvCloneImage(inImg);
	    for(int i = 1 ; i<= fgNum ; i++)
		{
			
			//if(fgRectArray[ i ].width > 40 && fgRectArray[ i ].height > 9 && ((float)fgRectArray[ i ].width)/fgRectArray[ i ].height>1.5) 
			//{
				
				cvRectangle(inImg , cvPoint(fgRectArray[ i ].x,fgRectArray[ i ].y), cvPoint(fgRectArray[ i ].x + fgRectArray[ i ].width,fgRectArray[ i ].y +fgRectArray[ i ].height),cvScalar( 255 , 20 , 250 ) , 2);
		    //}
		}
		for(int i=0;i< *orgNum;i++)
		cvRectangle(clone , cvPoint(BBox[ i ].x,BBox[ i ].y), cvPoint(BBox[ i ].x + BBox[ i ].width,BBox[ i ].y +BBox[ i ].height),cvScalar( 255 , 20 , 250 ) , 2);
		
		cvShowImage("gaurav",inImg);
		cvWaitKey(20);
		cvShowImage("saurav",clone);
		
		
		
		}	
        frameCount++;
	    }
        
       
	return 0;
}
