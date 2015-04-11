/******************************************************************************

   PROJECT:			CVDRIK
   FILE:			CVDRIKTextDetection.h
   AUTHOR:			KUMAR GAURAV
   DATE:            23rd Jan 2014
   DESCRIPTION:		Header for Text Recognition Functions

******************************************************************************/

#ifndef CVDRIKTEXTDETECTION_H
#define CVDRIKTEXTDETECTION_H

// Include Standard C/C++ Header Files Here
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Include OpenCV Header Files Here
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>

// Include CVDRIK Header Files Here
#include "../CVDRIKMultiChannelImage/CVDRIKMultiChannelImage.h"
#include "../CVDRIKMisc/CVDRIKMisc.h"


struct Point2d {
    int x;
    int y;
    float SWT;
};

struct Point2dFloat {
    float x;
    float y;
};

struct Ray {
        Point2d p;
        Point2d q;
        std::vector<Point2d> points;
};

struct Point3dFloat {
    float x;
    float y;
    float z;
};


struct Chain {
    int p;
    int q;
    float dist;
    bool merged;
    Point2dFloat direction;
    std::vector<int> components;
};

bool Point2dSort (Point2d const & lhs,
                  Point2d const & rhs);


bool checkHeadline(IplImage* inputImg);

bool computeRecog(IplImage* inImg);
bool computeTemporal(IplImage *frame1, IplImage *frame2);
int calchist(IplImage* inImg,int **hist);
float computeTest(IplImage* grayImg );
// Function for Computing Count of edge pixels
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numLine : It will contain the number of Lines in text box
//          (IplImage**) lineImg : An array of IplImages where each image corresponds to a particular line of text
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeLineSegmentation( &binImg , &numLine, &lineImg );
int computeCount(IplImage* binImg);

// Function for Computing Line Segmentation
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numLine : It will contain the number of Lines in text box
//          (IplImage**) lineImg : An array of IplImages where each image corresponds to a particular line of text
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeLineSegmentation( &binImg , &numLine, &lineImg );
unsigned char computeLineSegmentation( IplImage* binImg , int* numLine, IplImage** lineImg);

// Function for Computing Word Segmentation
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numWord : It will contain the number of words in text line
//          (IplImage**) wordImg : An array of IplImages where each image corresponds to a particular word of text
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeWordSegmentation( &binImg , &numWord, &wordImg );
unsigned char computeWordSegmentation( IplImage* binImg , int* numWord, IplImage** wordImg);

 // Function for Computing HOG
// Inputs :-
//			(IplImage*) inImg : Pointer to the 1-Channel Gray Image
// Outputs :-
//			(float*) hog : pointer to array hog
// Invoked As : float* hog  = computeHOG( &grayImg );
float* computeHOG(IplImage* grayImg);

// Function for Computing eLBP features
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numLine : It will contain the number of Lines in text box
//          (IplImage**) lineImg : An array of IplImages where each image corresponds to a particular line of text
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeLineSegmentation( &binImg , &numLine, &lineImg );
float* computeELBP(IplImage* grayImg);


// Function for Computing Statistical features
// Inputs :-
//			(IplImage*) grayImg : Pointer to the 1-Channel Gray Image
// Outputs :-
//			(float*) stat : pointer to array stat
// Invoked As : float* stat  = computeStatistical( &grayImg );

float* computeStatistical(IplImage* grayImg);

 // Function for Computing Rectangular Boxes
// Inputs :-
//			(IplImage*) inImg : Pointer to the 3-Channel Binary Image
//          (IplImage**) boxImg : Pointer to 3- channel boxes images
//          (CvRect*)  fgRectArray : Array of structures
//          (int) numBox : It will contain the number of Lines in text box
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeRectangularImages( &inImg , &boxImg, &fgRectArray,numBox );
CvRect* ComputeRectangularImages(IplImage* inImg,IplImage** boxImg, CvRect* fgRectArray,int numBox,int *orgNum);

// Function for Detcting Text in an image
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numLine : It will contain the number of Lines in text box
//          (IplImage**) lineImg : An array of IplImages where each image corresponds to a particular line of text
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = textDetection( &binImg , &numLine, &lineImg );
IplImage * textDetection (IplImage *    float_input, bool dark_on_light);

void strokeWidthTransform (IplImage * edgeImage,
                           IplImage * gradientX,
                           IplImage * gradientY,
                           bool dark_on_light,
                           IplImage * SWTImage,
                           std::vector<Ray> & rays);

void SWTMedianFilter (IplImage * SWTImage,
                     std::vector<Ray> & rays);

std::vector< std::vector<Point2d> >
findLegallyConnectedComponents (IplImage * SWTImage,
                                std::vector<Ray> & rays);

std::vector< std::vector<Point2d> >
findLegallyConnectedComponentsRAY (IplImage * SWTImage,
                                std::vector<Ray> & rays);

void componentStats(IplImage * SWTImage,
                                        const std::vector<Point2d> & component,
                                        float & mean, float & variance, float & median,
                                        int & minx, int & miny, int & maxx, int & maxy);

void filterComponents(IplImage * SWTImage,
                      std::vector<std::vector<Point2d> > & components,
                      std::vector<std::vector<Point2d> > & validComponents,
                      std::vector<Point2dFloat> & compCenters,
                      std::vector<float> & compMedians,
                      std::vector<Point2d> & compDimensions,
                      std::vector<std::pair<Point2d,Point2d> > & compBB );


std::vector<Chain> makeChains( IplImage * colorImage,
                 std::vector<std::vector<Point2d> > & components,
                 std::vector<Point2dFloat> & compCenters,
                 std::vector<float> & compMedians,
                 std::vector<Point2d> & compDimensions,
                 std::vector<std::pair<Point2d,Point2d> > & compBB);

#endif

