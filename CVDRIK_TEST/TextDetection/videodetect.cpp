//////////////////////////////////Main File////////////////////////////////
/*                      Text Detection Using SWT                         */
/*                      AUTHOR : KUMAR GAURAV                            */

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
#include "../../CVDRIK/CVDRIKMultiChannelImage/CVDRIKMultiChannelImage.h"
#include "../../CVDRIK/CVDRIKImageProcessing/CVDRIKImageProcessing.h"
#include "../../CVDRIK/CVDRIKTextDetection/CVDRIKTextDetection.h"

IplImage* loadByteImage ( const char* name )
{
  IplImage* image = cvLoadImage ( name );

  if ( !image )
  {
    return 0;
  }
  
  cvCvtColor ( image, image, CV_BGR2RGB );
  
  return image;
}

int mainTextDetection ( int argc, char** argv )
{


	// Video to frames

	//make directory to store cvrect of frames
	char sysStr[150] ; 
	sprintf( sysStr , "mkdir videoframes" );
	system( sysStr ) ; 

	//open video
	
	CvCapture *capture;
	capture= cvCreateFileCapture( argv[1] );
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
	printf( "\n\n video name %s\ntotal frames %d\n Size %dX%d \n",argv[1],totalFrames,size.width,size.height);
	
	cvGrabFrame(capture);//incorporate the check on reading image check output
    
    //creating temporary images for handling the frames
	IplImage* inImg = NULL;
	inImg = cvCreateImage( size , IPL_DEPTH_8U , 3 );
    
    int frameCount = 1;
	while(1<2)
	{
		
		inImg= cvQueryFrame( capture );//destroy image in loop
		if( !inImg ) break;
		cvShowImage("frames",inImg);
		cvWaitKey(60);
       //Saving Image
       char out[150];
       sprintf( out , "videoframes/frame_%d.png",frameCount);
       cvSaveImage(out,inImg);
       	if(frameCount>20)
       	{	
       cvCvtColor ( inImg, inImg, CV_BGR2RGB );

		// Detect text in the image

		 
		IplImage* output1 = textDetection ( inImg, 0 );  
		IplImage* output2 = textDetection ( inImg, 1);
		IplImage* output = cvCreateImage( cvGetSize ( inImg ) , IPL_DEPTH_8U , 3 );
  
		cvZero( output );
   	
	// Computing the Color Greying Over Pixels
	int baseIndx = -( output1->widthStep );
	int greyBaseIndx = - (output->widthStep);
	
	for( int y = 0 ; y < ( output1->height ) ; ++y )
	{
		// Update the Base Indices
		baseIndx = baseIndx + ( output1->widthStep );
		greyBaseIndx = greyBaseIndx + (output->widthStep);

		// Set the Current Indices
		int currIndx = baseIndx - 3;
		int currIndxGrey = greyBaseIndx - 1;

		// Looping Over the Columns
		for( int x = 0 ; x < ( output1->width ) ; ++x )
		{
			// Update the Current Indices
			currIndx = currIndx + 3;
			currIndxGrey = currIndxGrey + 3;			


			int blue1 = (unsigned char) output1->imageData[ currIndx ];
			int blue2 = (unsigned char) output2->imageData[ currIndx ];
			int blue3;
			if(blue1 < blue2 )
				blue3 = blue1;
			else
				blue3 = blue2;
			
            output->imageData[currIndxGrey] = (char)(blue3);
			int green1 = (unsigned char) output1->imageData[ currIndx + 1 ];
			int green2 = (unsigned char) output2->imageData[ currIndx + 1 ];
	        int green3;
			if(green1 < green2 )
				green3 = green1;
			else
				green3 = green2;
				
			output->imageData[currIndxGrey + 1] = (char)(green3);
			int red1 = (unsigned char) output1->imageData[ currIndx + 2 ];
			int red2 = (unsigned char) output2->imageData[ currIndx + 2 ];
			int red3 ;
			if(red1 < red2)
				red3 = red1;
			else
				red3 =red2;
			output->imageData[currIndxGrey + 2] = (char)(red3);

		}
	}
		//cvShowImage("output1",output1);
		//cvWaitKey(10);
		//cvShowImage("output2",output2);
		//cvWaitKey(10);
		cvShowImage("output",output);
		cvWaitKey(60);
        
	}
		//cvReleaseImage ( &inImg );
		//cvSaveImage ( argv[3], output );
		//cvReleaseImage ( &output );
		frameCount = frameCount + 1;
	}
  return 0;
}

int main ( int argc, char** argv )
{
 
  return mainTextDetection ( argc, argv );
       
}
