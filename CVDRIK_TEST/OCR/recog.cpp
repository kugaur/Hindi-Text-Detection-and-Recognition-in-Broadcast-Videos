// code for Only text recognition
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



int main( int argc , char** argv )
{
	
	// Check Usage
	if( argc != 2 )
	{
		// Show Purpose
		printf( "\n\n Purpose : text recognition in images \n\n" );
		
		// Show Usage
		printf( "\n\n Usage As : recog [ImageFileName] \n\n" );
		
		// Show Sample Command Line
		printf( "\n\n Sample Command Line : ./recog imagename.png  \n\n" );
		
		return( 0 );
	}

	IplImage* im_gray = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* binImg = cvCreateImage(cvGetSize(im_gray),IPL_DEPTH_8U,1);
	IplImage* edgeImg = cvCreateImage(cvGetSize(im_gray),IPL_DEPTH_8U,1);
    cvCanny(im_gray, edgeImg, 175, 320,3);
    cvThreshold(edgeImg, binImg, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    

	int width = binImg->width;
	int height = binImg->height;
	
	//Line Segmentation
	printf("\n Line Segmentation \n");
    int* numLine = (int*)malloc(sizeof(int));	
	IplImage** lineImg;
	lineImg = new IplImage*[10];
	cvShowImage("initial",binImg);
	cvWaitKey(30); 
	unsigned char erc = computeLineSegmentation( binImg , numLine, lineImg);
	if( erc != 0 )
	{
		printf( "\n\n Error Code = %d \n\n" , erc );
		getchar();
	}
  printf("Number of lines: %d \n", *numLine);
  
  // Word Segmentation
  printf("\n Word Segmentation \n");
  int* numWord = (int*)malloc(sizeof(int));
  IplImage** wordImg;
  wordImg = new IplImage*[40];
  for(int i =1; i<=(*numLine) ; i++)
  {
	cvShowImage("lines",lineImg[i]);
	cvWaitKey(20);
	//Saving Image
     char out[150];
     sprintf( out , "line_%d.png",i);
     cvSaveImage(out,lineImg[i]);
	getchar();  
	erc = computeWordSegmentation( lineImg[i] , numWord, wordImg);
	if( erc != 0 )
	{
		printf( "\n\n Error Code = %d \n\n" , erc );
		getchar();
	}
	printf("Number of words : %d\n", *numWord);
	for(int k=1; k<= (*numWord) ;k++)
	{
		cvShowImage("words",wordImg[k]);
		cvWaitKey(20);
		sprintf( out , "word_%d_%d.png",i,k);
		cvSaveImage(out,wordImg[k]);
		getchar();
		
		//Character Segmentation
		/*
		printf("Character Segmentation \n");
		int* numChar = (int*)malloc(sizeof(int));
		IplImage** charImg;
		charImg = new IplImage*[40];
		erc = computeCharacterSegmentation( wordImg[k] , numChar, charImg);
		if( erc != 0 )
		{
			printf( "\n\n Error Code = %d \n\n" , erc );
			getchar();
		}
		printf("Number of charcters : %d\n", *numChar);
		
		for(int m=1 ; m <=(*numChar) ; m++)
		{
			cvShowImage("out2",charImg[m]);
			cvWaitKey(20);
			sprintf( out , "char_%d_%d_%d.png",i,k,m);
			cvSaveImage(out,charImg[m]);
			getchar();			
			
		}
		
		 */
	}
	
	
  }
  

}
