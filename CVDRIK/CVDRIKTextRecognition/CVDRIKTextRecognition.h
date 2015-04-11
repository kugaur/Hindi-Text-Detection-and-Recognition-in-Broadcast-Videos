/******************************************************************************

   PROJECT:			CVDRIK
   FILE:			CVDRIKTextRecognition.h
   AUTHOR:			KUMAR GAURAV
   DATE:
   DESCRIPTION:		Header for Text Recognition Functions

******************************************************************************/

#ifndef __CVDRIKITEXTRECOGNITION_H__
#define __CVDRIKITEXTRECOGNITION_H__

/*************************** OTHER INCLUDE FILES *****************************/

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



/*****************************************************************************/


/******************************************************************************

					FUNCTIONS FOR TEXT RECOGNITION

******************************************************************************/

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


// Function for Computing Character Segmentation
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numCharacter : It will contain the number of characters in word
//          (IplImage**) charImg : An array of IplImages where each image corresponds to a particular character
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeCharcterSegmentation( &binImg , &numLine, &charImg );
unsigned char computeCharacterSegmentation( IplImage* binImg , int* numCharacter, IplImage** charImg);

// Function for Computing Feature Histogram of Projection based on mean distance
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (CVDRIKMultiChannelFloatImage*) projImg : Pointer to the 1-Channel Binary Image with pixel value computed using histogram projection based on mean distance
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeHistProjMean( &binImg , &projImg );
unsigned char computeHistProjMean( IplImage* binImg , CVDRIKMultiChannelFloatImage* projImg);


// Function for Computing Feature Histogram of Projection based on pixel value
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (float**) vertProj : Contains normalized number of black pixels in boundary image coulmn wise 
//          (float**) horizProj : Contains normalized number of black pixels in boundary image row wise
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeHistProjPixel( &binImg , &vertProj, &horizProj );
unsigned char computeHistProjPixel( IplImage* binImg , float** vertProj, float** horizProj);


// Function for Computing Feature Virtical Zero Crossing for Each Column
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (double**) numCross : numCross will contain number of virtical zero crossing
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeVirticalZeroCross( &binImg , &numCross );
unsigned char computeVirticalZeroCross( IplImage* binImg , double** numCross);

// Function for Computing Feature Horizontal Zero Crossing for Each Column
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (double**) numCross : numCross will contain normalized number of horizontal zero crossing
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeHorizontalZeroCross( &binImg , &numCross );
unsigned char computeHorizontalZeroCross( IplImage* binImg , double** numCross);


#endif
