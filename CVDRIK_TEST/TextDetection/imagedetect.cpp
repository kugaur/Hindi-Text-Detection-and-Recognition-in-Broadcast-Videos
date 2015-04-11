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
 

  IplImage* byteQueryImage = loadByteImage ( argv[1] );

  if ( !byteQueryImage )
  {
    printf ( "couldn't load query image\n" );
    return -1;
  }

  // Detect text in the image

  IplImage* output1 = textDetection ( byteQueryImage, 0 );
  IplImage* output11 = cvCreateImage( cvGetSize ( byteQueryImage ) , IPL_DEPTH_8U , 1 );
  IplImage* im_bw = cvCreateImage( cvGetSize ( byteQueryImage ) , IPL_DEPTH_8U , 1 );
  cvCvtColor (output1, output11, CV_RGB2GRAY);  
  cvThreshold(output11, im_bw, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); 
  IplImage* output2 = textDetection ( byteQueryImage, 1);
  IplImage* output22 = cvCreateImage( cvGetSize ( byteQueryImage ) , IPL_DEPTH_8U , 1 );
  IplImage* im_bw2 = cvCreateImage( cvGetSize ( byteQueryImage ) , IPL_DEPTH_8U , 1 );
  cvCvtColor (output2, output22, CV_RGB2GRAY);  
  cvThreshold(output22, im_bw2, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); 
  IplImage* output = cvCreateImage( cvGetSize ( byteQueryImage ) , IPL_DEPTH_8U , 1 );

  cvZero( output );
   	
	// Computing the Color Greying Over Pixels
	int baseIndx = -( output11->widthStep );
	int greyBaseIndx = - (output->widthStep);
	
	for( int y = 0 ; y < ( output11->height ) ; ++y )
	{
		// Update the Base Indices
		baseIndx = baseIndx + ( output11->widthStep );
		greyBaseIndx = greyBaseIndx + (output->widthStep);

		// Set the Current Indices
		int currIndx = baseIndx - 1;
		int currIndxGrey = greyBaseIndx - 1;

		// Looping Over the Columns
		for( int x = 0 ; x < ( output11->width ) ; ++x )
		{
			// Update the Current Indices
			currIndx = currIndx + 1;
			currIndxGrey = currIndxGrey + 1;			


			int value1 = (unsigned char) im_bw->imageData[ currIndx ];
			int value2 = (unsigned char) im_bw2->imageData[ currIndx ];
			int value3;
			if(value1 < value2 )
				value3 = value1;
			else
				value3 = value2;
			
            output->imageData[currIndxGrey] = (char)(value3);
			
			//if( red1<255 or green1 <255 or blue1<255)
			//printf("%d %d %d \n", red1,green1,blue1);
		}
	}
  	
  //cvReleaseImage ( &byteQueryImage );
  cvSaveImage ( argv[2], im_bw );
  //cvReleaseImage ( &im_bw );
  cvSaveImage ( argv[3], im_bw2 );
  //cvReleaseImage ( &im_bw2 );
  //cvSaveImage ( argv[4], output );
  //cvReleaseImage ( &output );
  cvSaveImage ( argv[4], output);
  cvReleaseImage ( &output );
  
  
  return 0;
}

int main ( int argc, char** argv )
{		

  return mainTextDetection ( argc, argv );
    
 
}
