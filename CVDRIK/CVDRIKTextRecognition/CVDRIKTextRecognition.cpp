/*********************************************************************************
 * 
 PROJECT:			CVDRIKTextDetection
 FILE:			    CVDRIKTextRecognition.cpp
 AUTHOR:			KUMAR GAURAV
 DATE:              19th Jan 2014
 DESCRIPTION:		CPP File for Text Recognition Functions
 
 *********************************************************************************/


/*************************** OTHER INCLUDE FILES *****************************/

#include "CVDRIKTextRecognition.h"

#define EPSILON 0.0001
#define PI 3.141593
#define VERYLARGENUM 1000000


/*****************************************************************************/


/******************************************************************************
 *                           FUNCTIONS                           *
 
 ******************************************************************************/


// Function for Computing Line Segmentation
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numLine : It will contain the number of Lines in text box
//          (IplImage**) lineImg : An array of IplImages where each image corresponds to a particular line of text
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeLineSegmentation( &binImg , &numLine, &lineImg );
unsigned char computeLineSegmentation( IplImage* binImg , int* numLine, IplImage** lineImg)
{
	
	// Check Inputs
	if((binImg->imageSize<=0) || (binImg->nChannels!=1))
	{
		// Error Code 1: Invalid Input
		return(1);
	}

	// Line Segmentation
	int th1 = 0;// may depend on font
	int baseIndx = -(binImg->widthStep);
	int count=0,flag=0, start=0,end=0;
	for(int y=0 ; y< (binImg->height) ; ++y)
	{
		// update base index
		baseIndx = baseIndx + (binImg->widthStep) ;	
		int currIndx = baseIndx - 1;
	    flag=0;
		for(int x =0 ; x < (binImg->width) ; ++x )
		{
			currIndx = currIndx +1;
			printf("%d\n",binImg->imageData[currIndx]);
			if( binImg->imageData[currIndx]<0)//>-1
			  {
				  flag=1;
				  break;
			  }			
		}
		
		if(flag==0 or (y==binImg->height -1) )
		{
			if(flag==1)
				end = binImg->height -1;
			else
				end = y-1;
			if( end - start > th1)
			{									
				count++;
			// Allocate Memory for the Line Image
				lineImg[count] = cvCreateImage( cvSize( binImg->width , end-start +  1  ) , IPL_DEPTH_8U , 1 );
				int bindex = (start-1)*(binImg->widthStep);
				int bindex1 = -binImg->widthStep;
				for(int l=start;l<=end;l++)
				{
					bindex = bindex + binImg->widthStep;
					bindex1 = bindex1 + binImg->widthStep;
					int cindex = bindex - 1;
					int cindex1 = bindex1 - 1; 
					for(int m=0;m< binImg->width;m++)
					{
						cindex++;
						cindex1++;
						lineImg[count]->imageData[cindex1] = binImg->imageData[cindex]; 
						
					}
				
				}
				start = y+1;
		     }
		     else 
			{
				start = y+1;			
			}	
		}
		
	}
		
		*numLine = count; 
    // Error Code 0 : All well
    return( 0 );	
}



// Function for Computing Word Segmentation
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numWord : It will contain the number of words in text line
//          (IplImage**) wordImg : An array of IplImages where each image corresponds to a particular word of text
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeWordSegmentation( &binImg , &numWord, &wordImg );
unsigned char computeWordSegmentation( IplImage* binImg , int* numWord, IplImage** wordImg)
{
	// Check Inputs
	if((binImg->imageSize<=0) || (binImg->nChannels!=1))
	{
		// Error Code 1: Invalid Input
		return(1);
	}

	// Word Segmentation
	int th1 =20;
	int baseIndx = -1;
	int widthStep = binImg->widthStep;
	int count=0,flag=0, start=0,end=0;
	for(int x=0 ; x< (binImg->width) ; ++x)
	{
		// update base index
		baseIndx = baseIndx + 1 ;
		
		int currIndx = baseIndx - widthStep;

		flag=0;
		for(int y =0 ; y < (binImg->height) ; ++y )
		{
			currIndx = currIndx +widthStep;
			
			if( binImg->imageData[currIndx]<0)//>-1
			  {
				  flag=1;
				  break;
			  }			
		}
		if(flag==0 or (x == binImg->width -1))
		{
			if(flag==1)
				end = binImg->width -1;
			else
				end = x-1;
			if( end - start > th1)
			{				
				count++;
			// Allocate Memory for the Word Image
				wordImg[count] = cvCreateImage( cvSize( end-start + 2 , binImg->height) , IPL_DEPTH_8U , 1 );
				int bindex = -binImg->widthStep;
				int nwidthStep = end - start + 1 + 4 - (end-start + 1)%4;
				
				int bindex1 = -nwidthStep;
				for(int l=0;l< binImg->height;l++)
				{
					bindex = bindex + binImg->widthStep;
					bindex1 = bindex1 + nwidthStep;
					int cindex = bindex + start - 1;
					int cindex1 = bindex1 - 1; 
					for(int m=0;m<nwidthStep;m++)
					{
						cindex = cindex + 1;
						cindex1 = cindex1 + 1;
						wordImg[count]->imageData[cindex1] = binImg->imageData[cindex]; 
						
					}
				
				}
				
				start = x + 1;
		     }
		     else 
			{
				start = x + 1;			
			}	
		}
	
	}
		
		*numWord = count;
		 
    // Error Code 0 : All well
    return( 0 );	
}


// Function for Computing Character Segmentation
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (int*) numChar : It will contain the number of characters in word
//          (IplImage**) charImg : An array of IplImages where each image corresponds to a particular character
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeCharcterSegmentation( &binImg , &numChar, &charImg );
unsigned char computeCharacterSegmentation( IplImage* binImg , int* numChar, IplImage** charImg)
{
	
	// Check Inputs
	if((binImg->imageSize<=0) || (binImg->nChannels!=1))
	{
		// Error Code 1: Invalid Input
		return(1);
	}
	
	// Character Segmentation
	
	//Finding Headline
	
	int widthStep = binImg->widthStep;
	int baseIndx = -(binImg->widthStep);
	int *count= new int[binImg->height];
	int max=0;
	for(int y=0 ; y< (binImg->height) ; ++y)
	{
		// update base index
		baseIndx = baseIndx + (binImg->widthStep) ;
		
		int currIndx = baseIndx - 1;
	    
	    count[y]=0;
		for(int x =0 ; x < (binImg->width) ; ++x )
		{
			currIndx = currIndx +1;
			
			if( binImg->imageData[currIndx]>-1)
			  {
				  count[y]++;//white -1
				  
			  }			
		}
		if(count[y] > count[max])
		  max=y;		// max stores the row number
	}
	printf("headline : %d\n",max);
	
	// Removing Headline
	baseIndx = -binImg->widthStep;
    for(int y=0 ; y< (binImg->height) ; ++y)
	{
		// update base index
	   baseIndx = baseIndx + (binImg->widthStep) ;
		
	   int currIndx = baseIndx - 1;
	   
	   if(count[y]>= ((count[max]*9)/10 ))
	   {
			for(int x =0 ; x < (binImg->width) ; ++x )
			{
				currIndx = currIndx +1;
				binImg->imageData[currIndx]=-1;//whitening	
		    }
	   }		
	}
	// show word without headline
	cvShowImage("out4",binImg);
	cvWaitKey(20);
	getchar();	
	
	// Segmenting individual characters
	int charcount=0;			
	int th1 =2;
	baseIndx = -1;
	int flag1=0,flag2=0, start1=0,start2=0,end1=0,end2=0;
	for(int x=0 ; x< (binImg->width) ; ++x)
	{
		// update base index
		baseIndx = baseIndx + 1 ;		
		int currIndx = baseIndx - widthStep;
		flag1=0;
		flag2=0;
		for(int y1 = 0; y1 < max ; y1++)
		{
			currIndx = currIndx + widthStep;
			if( binImg->imageData[currIndx] > -1)
			{
				flag1 =1;
				break;
			}
		}
		for(int y =max ; y < (binImg->height) ; ++y )
		{
			currIndx = currIndx +widthStep;
			
			if( binImg->imageData[currIndx]> -1)
			  {
				  flag2=1;
				  break;
			  }			
		}
	    if(flag2==0 or (end2 ==0 and x == binImg->width - 1) )
		{
			if(flag2 ==1)
				end2 = binImg->width -1;
			else 
				end2 = x -1;
			if( end2 - start2 > th1)
			{	
								
				charcount++;
			// Allocate Memory for the Charcater Image
				charImg[charcount] = cvCreateImage( cvSize( end2-start2 + 2 , binImg->height - max ) , IPL_DEPTH_8U , 1 );
				int bindex = -binImg->widthStep;
				int nwidthStep = end2 - start2 + 1 + 4 - (end2-start2 + 1)%4;
				
				int bindex1 = -nwidthStep;
				for(int l=max;l< binImg->height;l++)
				{
					bindex = bindex + binImg->widthStep;
					bindex1 = bindex1 + nwidthStep;
					int cindex = bindex + start2 - 1;
					int cindex1 = bindex1 - 1; 
					for(int m=0;m<nwidthStep;m++)
					{
						cindex = cindex + 1;
						cindex1 = cindex1 + 1;
						charImg[charcount]->imageData[cindex1] = binImg->imageData[cindex]; 
						
					}
				
				}				
				start2 = x + 1;
		     }
		     else 
			{
				start2 = x + 1;			
			}	
		}
		if(flag1==0 or (end1 == 0 and x == binImg->width -1))
		{
			if(flag1==1)
			 end1 = binImg->width -1;
			else
				end1 = x -1;
			if( end1 - start1 > th1)
			{					
				charcount++;
			// Allocate Memory for the Charcater Image
				charImg[charcount] = cvCreateImage( cvSize( end1-start1 + 2 , max) , IPL_DEPTH_8U , 1 );
				int bindex = -binImg->widthStep;
				int nwidthStep = end1 - start1 + 1 + 4 - (end1-start1 + 1)%4;
				
				int bindex1 = -nwidthStep;
				for(int l=0;l< max;l++)
				{
					bindex = bindex + binImg->widthStep;
					bindex1 = bindex1 + nwidthStep;
					int cindex = bindex + start1 - 1;
					int cindex1 = bindex1 - 1; 
					for(int m=0;m<nwidthStep;m++)
					{
						cindex = cindex + 1;
						cindex1 = cindex1 + 1;
						charImg[charcount]->imageData[cindex1] = binImg->imageData[cindex]; 
						
					}
				
				}				
				start1 = x + 1;
		     }
		     else 
			{
				start1 = x + 1;			
			}	
		}
	
	}
			
	*numChar = charcount; 
    // Error Code 0 : All well
    return( 0 );	
}

// Function for Computing Feature Histogram of Projection based on mean distance
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (CVDRIKMultiChannelFloatImage*) projImg : Pointer to the 1-Channel Binary Image with pixel value computed using histogram projection based on mean distance
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeHistProjMean( &binImg , &projImg );
unsigned char computeHistProjMean( IplImage* binImg , CVDRIKMultiChannelFloatImage* projImg)
{
	// Check Inputs
	if((binImg->imageSize<=0) || (binImg->nChannels!=1) )
	{
		// Error Code 1: Invalid Input
		return(1);
	}
	
	// Computing Mean point
	int xmean=0,ymean=0;
	int baseIndx = -( binImg->widthStep );
	for( int y = 0 ; y < ( binImg->height ) ; ++y )
	{
		// Update the Base Indices
		baseIndx = baseIndx + ( binImg->widthStep );

		// Set the Current Indices
		int currIndx = baseIndx - 1;


		// Looping Over the Columns
		for( int x = 0 ; x < ( binImg->width ) ; ++x )
		{
			// Update the Current Indices
			currIndx = currIndx + 1;
			if( binImg->imageData[currIndx]==0)
			{
				xmean = xmean + x;
				ymean = ymean + y;  				
			}
		}
	}
	
	xmean = xmean/((binImg->width)*(binImg->height));
	ymean = ymean/((binImg->width)*(binImg->height));
	
	// Computing distance from mean for each pixel
	float maxdist = 0;
	baseIndx = -( binImg->widthStep );
	for( int y = 0 ; y < ( binImg->height ) ; ++y )
	{
		// Update the Base Indices
		baseIndx = baseIndx + ( binImg->widthStep );

		// Set the Current Indices
		int currIndx = baseIndx - 1;


		// Looping Over the Columns
		for( int x = 0 ; x < ( binImg->width ) ; ++x )
		{
			// Update the Current Indices
			currIndx = currIndx + 1;
			projImg->m_Img[currIndx] = sqrt((x-xmean)*(x-xmean) + (y-ymean)*(ymean));
			if(projImg->m_Img[currIndx] > maxdist)
				  maxdist = projImg->m_Img[currIndx];  				
			
		}		
	}
	//Normalization
	baseIndx = -( binImg->widthStep );
	for( int y = 0 ; y < ( binImg->height ) ; ++y )
	{
		// Update the Base Indices
		baseIndx = baseIndx + ( binImg->widthStep );

		// Set the Current Indices
		int currIndx = baseIndx - 1;


		// Looping Over the Columns
		for( int x = 0 ; x < ( binImg->width ) ; ++x )
		{
			// Update the Current Indices
			currIndx = currIndx + 1;			
			projImg->m_Img[currIndx] = projImg->m_Img[currIndx] / maxdist; 				
			
		}		
	}
		
   // Error Code 0 : All well
    return( 0 );	
}

// Function for Computing Feature Histogram of Projection based on pixel value
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (float**) vertProj : Contains normalized number of black pixels in boundary image coulmn wise 
//          (float**) horizProj : Contains normalized number of black pixels in boundary image row wise
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeHistProjPixel( &binImg , &vertProj, &horizProj );
unsigned char computeHistProjPixel( IplImage* binImg , float** vertProj, float** horizProj)
{
	// Check Inputs
	if((binImg->imageSize<=0) || (binImg->nChannels!=1) )
	{
		// Error Code 1: Invalid Input
		return(1);
	}
	
	//Computing Edge of Character Image using Canny Edge Detector
		//open the parameter file
	FILE* fp;
	fp = fopen( "CannyParameter.txt" , "r" );
	
	// Reading the Canny Parameter File
	char paramNameString[100] , eqString[3] , headerString[100];
	fscanf( fp , "%s" , headerString );
	int cannyTh1 = 0;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &cannyTh1 );
	int cannyTh2 = 0;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &cannyTh2 );
	int cannyWinSize = 0;
	fscanf( fp , "%s%s%d" , paramNameString , eqString , &cannyWinSize );
	IplImage* edgeImg = NULL;
	edgeImg = cvCreateImage( cvSize( binImg->width , binImg->height)  , IPL_DEPTH_8U , 1 );	
	cvCanny( binImg , edgeImg , cannyTh1 , cannyTh2 , cannyWinSize ); 
	
	fclose(fp);
	
	// Horizontal Profile
		
	int baseIndx = -(binImg->widthStep);
	float *hcount = new float[binImg->height]; 
	float max=0;
	for(int y=0 ; y< (binImg->height) ; ++y)
	{
		// update base index
		baseIndx = baseIndx + (binImg->widthStep) ;
		
		int currIndx = baseIndx - 1;
	    hcount[y]=0;
		for(int x =0 ; x < (binImg->width) ; ++x )
		{
			currIndx = currIndx +1;
			
			if( binImg->imageData[currIndx]<1)
			  {
				  hcount[y] = hcount[y] + 1;;
			  }			
		}
		if(max < hcount[y])
		  max = hcount[y];
	}
	
	for(int i=0; i< binImg->height ; i++)
	{
		hcount[i] = hcount[i] / max;		
	}
		
	// Vertical Projection
	
	max=0;
	float *vcount = new float[binImg->width];
	baseIndx = -1;
	for(int x=0 ; x< (binImg->width) ; ++x)
	{
		// update base index
		baseIndx = baseIndx + 1 ;
		
		int currIndx = baseIndx - binImg->widthStep;
		vcount[x]=0;
		for(int y =0 ; y < (binImg->height) ; ++y )
		{
			currIndx = currIndx + binImg->widthStep;
			
			if( binImg->imageData[currIndx]<1)
			  {				  
				  vcount[x] = vcount[x] + 1;				  
			  }			
		}
		if(max < vcount[x] )
		  max = vcount[x];
	    
	    
	}	
	
	for(int i=0; i< binImg->width ; i++)
	{
		vcount[i] = vcount[i] / max;		
	}
		
	*vertProj = vcount;
	*horizProj = hcount;
		
    // Error Code 0 : All well
    return( 0 );
	
}

// Function for Computing Feature Virtical Zero Crossing for Each Column
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (double**) numCross : numCross will contain number of virtical zero crossing
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeVirticalZeroCross( &binImg , &numCross );
unsigned char computeVirticalZeroCross( IplImage* binImg , double** numCross)
{
	// Check Inputs
	if((binImg->imageSize<=0) || (binImg->nChannels!=1) )
	{
		// Error Code 1: Invalid Input
		return(1);
	}
	
	// Traversing column wise
	double *count = new double[binImg->width];			
	int baseIndx = -1;
	int max =0 ;
	int flag=0, start=0,end;
	for(int x=0 ; x< (binImg->width) ; ++x)
	{
		// update base index
		baseIndx = baseIndx + 1 ;
		
		int currIndx = baseIndx - binImg->widthStep;
		count[x]=0;
		for(int y =0 ; y < (binImg->height) ; ++y )
		{
			currIndx = currIndx +binImg->widthStep;
			
			if( binImg->imageData[currIndx]<1)
			  {
				  if(currIndx + binImg->widthStep < (binImg->width)*(binImg->height))
				  {
					  if(binImg->imageData[currIndx + binImg->widthStep] ==1)
					  count[x]++;					  
				  }
			  }			
		}
		if(max< count[x])
		  max = count[x];
	
	}		
	for(int i = 0; i < binImg->width ; ++i )
	 count[i] = count[i] / max;
 	
 	*numCross = count;
	// Error Code 0: All is well
	return(0);
}


// Function for Computing Feature Horizontal Zero Crossing for Each Column
// Inputs :-
//			(IplImage*) binImg : Pointer to the 1-Channel Binary Image
//          (double**) numCross : numCross will contain number of horizontal zero crossing
// Outputs :-
//			(unsigned char) errCode : The Error Code of Execution
// Invoked As : errCode = computeHorizontalZeroCross( &binImg , &numCross );
unsigned char computeHorizontalZeroCross( IplImage* binImg , double** numCross)
{
	
		// Check Inputs
	if((binImg->imageSize<=0) || (binImg->nChannels!=1) )
	{
		// Error Code 1: Invalid Input
		return(1);
	}
	int max=0;
	double *count = new double[binImg->height];
	int baseIndx = -( binImg->widthStep );
	for( int y = 0 ; y < ( binImg->height ) ; ++y )
	{
		// Update the Base Indices
		baseIndx = baseIndx + ( binImg->widthStep );

		// Set the Current Indices
		int currIndx = baseIndx - 1;
		count[y]=0;
		// Looping Over the Columns
		for( int x = 0 ; x < ( binImg->width ) ; ++x )
		{
			// Update the Current Indices
			currIndx = currIndx + 1;
			if( binImg->imageData[currIndx]==0)
			{
				if(currIndx + 1 < baseIndx + (binImg->widthStep))
				 {
					 if(binImg->imageData[currIndx]==1)
					   count[y]++;					 
				 }		
			}
			
		}
		if(count[y]>max)
		   max =count[y];
	}
	// Normalization
	for( int i=0; i < binImg->height; ++i)
	{
		count[i] = count[i]/max;		
	}	
	*numCross = count;
	
// Error Code 0: All is well
	return(0);
	
}


