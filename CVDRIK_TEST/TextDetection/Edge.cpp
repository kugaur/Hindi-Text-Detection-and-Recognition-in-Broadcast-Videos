
//////////////////////////////////Main File////////////////////////////////
/*                      Text Detection Using SWT                         */
/*                      AUTHOR : KUMAR GAURAV                            */

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
    // Allocate Memory for the Edge Image
	IplImage* edgeImg = NULL;
	edgeImg = cvCreateImage( size , IPL_DEPTH_8U , 1 );
	IplImage* colorImg =NULL;
	colorImg = cvCreateImage( size , IPL_DEPTH_8U , 3 );
    int frameCount = 1;
    // Perform Color Gradient Image Computation
	double fps = 0.0;
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
       	
       	cvCanny(inImg, edgeImg,175,320,3);
       	cvCvtColor(edgeImg,colorImg,CV_GRAY2BGR);
       	std::vector<cv::Vec3f> lines;
       	cvHoughLines2(edgeImg, lines,1,CV_PI/180, 100);
       	
       	/*unsigned char erc = computeEdgeColorImage( inImg, edgeImg, 0.114, 0.587, 0.299, 1 );
		if( erc != 0 )
		{
			printf( "\n\n Error Code = %d \n\n" , erc );
			getchar();
		}
       */
   	
	
		//cvShowImage("output1",output1);
		//cvWaitKey(10);
		//cvShowImage("output2",output2);
		//cvWaitKey(10);
		cvShowImage("output",edgeImg);
		cvWaitKey(60);
        
	
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
