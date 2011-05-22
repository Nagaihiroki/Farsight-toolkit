

/* 
 * Copyright 2009 Rensselaer Polytechnic Institute
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#include "cell_binarization.h"
#include <limits.h>
#include <math.h>
#include <time.h>

#include "ftkLaplacianOfGaussian3D.h"

#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"


#ifdef _OPENMP
#include "omp.h"
#endif

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)-1)
#endif

//#include "maxflow.cpp"
//#include "graph.cpp"


using namespace std;

//Main function for 2-D binarization
int Cell_Binarization_2D(unsigned char* imgIn, unsigned short *imgOut, int R, int C, int shd)
{			
	//Now, to do the binarization, follow these steps:
	//1- Assuming that the histogram of the image is modeled by a mixture of two 
	//poisson distributions, estimate the parameters of the mixture model 
	float alpha_B, alpha_F, P_I;
	alpha_B = alpha_F = P_I = 0;	
	//MinErrorThresholding(imgIn, &alpha_B, &alpha_F, &P_I, R, C, 1, shd,imgOut); 		
	float alpha_C, P_I2;
	alpha_C = P_I2 = 0.0;
	threeLevelMinErrorThresh(imgIn, &alpha_B, &alpha_F, &alpha_C, &P_I, &P_I2, R, C, 1);

	//2- Apply binarization refinement using graph-cuts	
	int n_nodes, n_edges;
	//alpha_C = -1;
	if(alpha_C == -1)//this is a two levels case
	{
		//graph-cuts
		Seg_GC_Full_2D(imgIn, R, C, alpha_F, alpha_B, P_I, &n_nodes, &n_edges, imgOut);
	}
	else  //three levels case
	{
		//try this
		alpha_C = (alpha_C+alpha_F)/2;
		alpha_B = (alpha_B+alpha_F)/2;
		//graph-cuts
		Seg_GC_Full_2D_Three_Level(imgIn, R, C, alpha_C,alpha_B, alpha_F, P_I, P_I2,&n_nodes, &n_edges, imgOut);
	}		
	
	return 1;	
}

//Main function for 3-D binarization
int Cell_Binarization_3D(unsigned char *imgIn, unsigned short* imgOut, int R, int C, int Z, int shd, int div) //modifed by Yousef on 5-20-2008.. The first input change from uchar* to int*
{			
	/*double*** image_3D = (double***) malloc(C * sizeof(double **));
	for (int k = 0; k < C; k++)
	{
		image_3D[k] = (double **) malloc(R * sizeof(double *));
		for (int l = 0; l < R; l++)
		{
			image_3D[k][l] = (double *) malloc(Z * sizeof(double));
			for (int m = 0; m < Z; m++)
			{
				image_3D[k][l][m] = imgIn[k + l * C + m * R * C];
			}
		}
	}

	for (double scale = 1; scale <= 20; scale += 1)
	{
		double*** out_image = runLoG(image_3D, scale, scale, scale, C, R, Z); //LoG

		//rescale out_image
		double max = numeric_limits<double>::min();
		double min = numeric_limits<double>::max();

		for (int k = 1; k < C - 1; k++)
			for (int l = 1; l < R - 1; l++)
				for (int m = 0; m < Z; m++)
				{
					if (out_image[k][l][m] > max)
						max = out_image[k][l][m];
					if (out_image[k][l][m] < min)
						min = out_image[k][l][m];
				}
		
		for (int k = 0; k < C; k++)
			for (int l = 0; l < R; l++)
				for (int m = 0; m < Z; m++)
					out_image[k][l][m] = (out_image[k][l][m] - min) * 255 / (max - min);

		std::string outputFilename = "LoG";
		std::stringstream tempStringStream;
		tempStringStream << scale;
		outputFilename.append(tempStringStream.str());
		outputFilename.append(".tif");

		typedef unsigned char OutputPixelType;
		const unsigned int Dimension = 3;
		typedef itk::Image<OutputPixelType, Dimension > ImageType;

		ImageType::RegionType region;
		ImageType::IndexType start;
		
		start[0] = 0;
		start[1] = 0;
		start[2] = 0;

		ImageType::SizeType size;
		size[0] = C;
		size[1] = R;
		size[2] = Z;

		region.SetSize(size);
		region.SetIndex(start);

		ImageType::Pointer image = ImageType::New();
		image->SetRegions(region);
		image->Allocate();

		ImageType::IndexType pixelIndex;

		for (int k = 0; k < C; k++)
		{
			for (int l = 0; l < R; l++)
			{
				for (int m = 0; m < Z; m++)
				{
					pixelIndex[0] = k;
					pixelIndex[1] = l;
					pixelIndex[2] = m;

					image->SetPixel(pixelIndex, out_image[k][l][m]);
				}
				free(out_image[k][l]);
			}
			free(out_image[k]);
		}
		free(out_image);
		

		typedef  itk::ImageFileWriter< ImageType  > WriterType;
		WriterType::Pointer writer = WriterType::New();
		writer->SetFileName(outputFilename);
		writer->SetInput(image);
		writer->Update();
	}*/


	
	//Now, to do the binarization, follow these steps:
	//1- Assuming that the histogram of the image is modeled by a mixture of two 
	//poisson distributions, estimate the parameters of the mixture model 
	float alpha_B, alpha_F, P_I;
	alpha_B = alpha_F = P_I = 0;	
	//CompMixPoss3D(imgIn, &alpha_B, &alpha_F, &P_I, R, C, Z, shd); 	
	MinErrorThresholding(imgIn, &alpha_B, &alpha_F, &P_I, R, C, Z, shd,imgOut); 	
	
	//2- Use graph cuts to binarize the image
	//this function will start by graph building (learning step) and then it will 
	//do the inference step (max-flow)	
	//Seg_GC_Full_3D(imgIn, R, C, Z, alpha_F, alpha_B, P_I, imgOut);
	//cout<<"Finalizing Binarization..";
	//post_binarization(imgOut, 2, 3, R, C, Z);
	//cout<<"done"<<endl;
	
	int block_divisor;
	
	#ifdef _OPENMP			
		block_divisor = 8;	
	#else		
		block_divisor = 8;	
	#endif

	
	std::cerr << "block_divisor = " << block_divisor << std::endl;

	int cntr = 0;
	for(int i=0; i<R; i+=R/block_divisor)
		for(int j=0; j<C; j+=C/block_divisor)
			cntr++;	

//May make sense to trade some accuracy for speed if we have enough memory (parallelize each subimage) -Ho (2/18/2011)	
	std::cout<<"Total Blocks: "<<cntr<<std::endl;	

	int blk = 1;
	
	clock_t start_time_cell_bin_alpha_exp = clock();
	int ***subImgBlockArray = (int ***) malloc(R * sizeof(int **));

	#ifdef _OPENMP
		omp_set_nested(1);
	#endif
	
	#pragma omp parallel for
	for (int i = 0; i < R; i+=R/block_divisor)
	{
		subImgBlockArray[i] = (int **) malloc(C * sizeof(int *));
		#pragma omp parallel for
		for (int j = 0; j < C; j+=C/block_divisor) 
			subImgBlockArray[i][j] = (int *) malloc(6 * sizeof(int));
	}
	

	
	#pragma omp parallel for
	for(int i=0; i<R; i+=R/block_divisor)
	{			
		#pragma omp parallel for
		for(int j=0; j<C; j+=C/block_divisor)
		{			
			subImgBlockArray[i][j][4] = 0;
			subImgBlockArray[i][j][5] = Z - 1;
			subImgBlockArray[i][j][0] = j;
			subImgBlockArray[i][j][1] = (int)j+C/block_divisor-1;
			subImgBlockArray[i][j][2] = i;
			subImgBlockArray[i][j][3] = (int)i+R/block_divisor-1;
			if(subImgBlockArray[i][j][1] > C - 1)
				subImgBlockArray[i][j][1] = C - 1;
			if(subImgBlockArray[i][j][3] > R - 1)
				subImgBlockArray[i][j][3] = R - 1;
		}
	}

	#pragma omp parallel for
	for(int i=0; i<R; i+=R/block_divisor)
	{			
		//#pragma omp parallel for
		for(int j=0; j<C; j+=C/block_divisor)
		{
			//#pragma omp critical
			{
				//std::cout<<"    Binarizing block " << blk++ <<" of "<<cntr<<std::endl;	
			}
			Seg_GC_Full_3D_Blocks(imgIn, R, C, Z, alpha_F, alpha_B, P_I, imgOut, subImgBlockArray[i][j]); //imgIn is dataImagePtr, imgOut is binImagePtr
		}
	}

	/*#ifdef _OPENMP
		omp_set_nested(1);
	#endif*/
	
	#pragma omp parallel for
	for(int i=0; i<R; i+=R/block_divisor)
	{			
		#pragma omp parallel for
		for(int j=0; j<C; j+=C/block_divisor)
		{
			free(subImgBlockArray[i][j]);
		}
		free(subImgBlockArray[i]);
	}
	free(subImgBlockArray);

	subImgBlockArray = NULL;

	

	#ifdef _OPENMP
		omp_set_nested(0);
	#endif

	cout << "Cell Binarization refinement by alpha expansion took " << (clock() - start_time_cell_bin_alpha_exp)/(float)CLOCKS_PER_SEC << " seconds" << endl;

	return 1;//num_objects;	
}


double compute_poisson_prob(double intensity, double alpha)
{
    /*this is the equation
      P = (alpha^intensity)*exp(-alpha)/factorial(intensity);
      however, since alpha and the intensity could be large, computing P in that
      way will result in infinity values from (alpha^intensity) and
      factorial(intensity) as a result of matlab's limitations of the data types*/

    //here is the solution
    double A, P;
    A = exp(-alpha);
    P = 1;
    for (int i=1; i<= intensity; i++)
        P = P * (alpha/i);
    
    P = P*A;

	if(P < numeric_limits<long double>::epsilon())
		P = numeric_limits<long double>::epsilon();
    
    return P;
}

void Seg_GC_Full_2D(unsigned char* IM,
                    int r, 
                    int c, 
                    double alpha_F, 
                    double alpha_B, 
                    double P_I, 
                    int* num_nodes, 
                    int* num_edges, 
                    unsigned short* Seg_out)
{    
    int curr_node;
    int rght_node;
    int down_node;
    int diag_node;
    double Df;
    double Db;
    double Dr;
    double Dd; 
    double Dg; 
    double sig;
    double F_H[256];
    double B_H[256];
    typedef Graph_B<int,int,int> GraphType;
       
    //Set the number of edges and the number of nodes and open the files that
    //will be used to save the weights
    num_nodes[0] = r*c;
    num_edges[0] = 3*r*c-2*r-2*c+1;
        	
    
    //Before entering the loop, compute the poisson probs 
    for(int i=0; i<256; i++)
    {
		if(i>=alpha_F)
			F_H[i] = (1-P_I)*compute_poisson_prob((int)alpha_F,alpha_F);
		else
			F_H[i] = (1-P_I)*compute_poisson_prob(i,alpha_F);
		if(i<=alpha_B)
			B_H[i] = P_I*compute_poisson_prob(int(alpha_B),alpha_B);
		else
        B_H[i] = P_I*compute_poisson_prob(i,alpha_B);
    }
	
    //Here is the main loop.. 
    //For each point, compute the terminal and neighbor edge weights
	GraphType *g = new GraphType(/*estimated # of nodes*/ num_nodes[0], /*estimated # of edges*/ num_edges[0]); 
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {			
			/*Get the terminal edges capacities and write them to a file
			These capacities represent penalties of assigning each point to
			either the fg or the bg. Now, I am using the distance of the point
			to the fg and bg. Then a short distance between a point and a class (fg or bg)
			means the penalty of the assignement is low and vice versa*/ 
								
		   //Isaac change on 5/9/08
		   //int intst = (int) IM[i][j];
			int intst = (int) IM[i*c + j];

            //Added by Yousef on jan 17, 2008
            //check if this is a seed point
            if(intst == 255)
            {
                Df = 0;
                Db = 1000;
            }
            else if(intst == 0)
            {
                Df = 1000;
                Db = 0;
            }
            else
            {                
                Df = -log(F_H[intst]);  //it was multiplied by .5          
                if(Df>1000.0)
                    Df = 1000;
                Db = -log(B_H[intst]);
                if(Db>1000.0)
                    Db=1000;
                
            }            
            
            curr_node = (i*c)+j; 
            
			g -> add_node();
			g -> add_tweights( curr_node,   /* capacities */ Df,Db);                            			
		}
	}

    sig = 15.0;
	double w = 2.0;
    for(int i=0; i<r-1; i++)
      {
      for(int j=0; j<c-1; j++)
        {			
        // get the neighbor edges capacities and write them to a file.
	// Now, each edge capacity between two neighbors p and q represent
	// the penalty for discontinuety. Since I am using the difference in
	// the intensities, I should take the inverse so that very similar
	// objects should have a large discontinuety penalty between them*/

		curr_node = (i*c)+j; 
        rght_node = curr_node+1;
        down_node = curr_node+c;
        diag_node = curr_node+c+1;
            
            
        //from Boykov's paper instead
		Dr = w*exp(-pow((double)IM[i*c + j]-(double)IM[i*c + j+1],2)/(2*pow(sig,2)));
		g -> add_edge( curr_node, rght_node,    /* capacities */  Dr, Dr );	
            
                   
        //from Boykov's paper instead
		Dd = w*exp(-pow((double)IM[i*c + j]-(double)IM[(i+1)*c + j],2)/(2*pow(sig,2)));
		g->add_edge( curr_node, down_node,    /* capacities */  Dd, Dd );
                 
		Dg = w*exp(-pow((double)IM[i*c + j]-(double)IM[(i+1)*c + (j+1)],2)/(2*pow(sig,2)));
		g->add_edge( curr_node, diag_node,    /* capacities */  Dg, Dg );            
               
        }
      }    
	
    //Compute the maximum flow:
	g->maxflow();		//Alex DO NOT REMOVE


	int RR,CC;
	for(int i=0; i<num_nodes[0]; i++)
	{
		CC = ((long)i)%c;
        RR = (i-CC)/c;
		if(g->what_segment(i) == GraphType::SOURCE)
			Seg_out[RR*c + CC]=0;
		else
			Seg_out[RR*c + CC]=255;
	}

	delete g;
}


void Seg_GC_Full_3D(unsigned char* IM, int r, int c, int z, double alpha_F, double alpha_B, double P_I, int* Seg_out)
{   
    int curr_node, nbr_node;
    double Df, Db, Dn, sig, w;
    double F_H[256], B_H[256];
	double intst, intst2;
    unsigned long int num_nodes, num_edges;
	typedef Graph_B<short,short,short> GraphType;  
      
    //Set the number of edges and the number of nodes and open the files that
    //will be used to save the weights
    num_nodes = r*c*z;
    num_edges = (r-1)*(c-1)*(z-1)*3;   
        
    //Before entering the loop, compute the poisson probs    
	for(int i=0; i<256; i++)
    {
		if(i>=alpha_F)
			F_H[i] = (1-P_I)*compute_poisson_prob((int)alpha_F,alpha_F);
		else
			F_H[i] = (1-P_I)*compute_poisson_prob(i,alpha_F);
		if(i<=alpha_B)
			B_H[i] = P_I*compute_poisson_prob(int(alpha_B),alpha_B);
		else
        B_H[i] = P_I*compute_poisson_prob(i,alpha_B);
    }
	//std::cerr << "Poisson Probabilities Computed" << std::endl;
    
	//Construct the graph
	GraphType *g = new GraphType(/*estimated # of nodes*/ num_nodes, /*estimated # of edges*/ num_edges); 

	//std::cerr << "Graph Memory Allocated" << std::endl;

	//Here is the main loop.. 
    //For each point, compute the terminal and neighbor edge weights
	for(int k=0; k<z; k++)
    {		
        for(int j=0; j<r; j++)
        {			
			for(int i=0; i<c; i++)
			{
				
				/*Get the terminal edges capacities and write them to a file
				These capacities represent penalties of assigning each point to
				either the fg or the bg. Now, I am using the distance of the point
				to the fg and bg. Then a short distance between a point and a class (fg or bg)
				means the penalty of the assignement is low and vice versa*/ 
								

				//unsigned long int index = mxCalcSingleSubscript(prhs[0], nsubs, subs);
				//intst = (mxGetPr(prhs[0]))[index];
				// CHANGE BY ISAAC 4/14/08
				
				//CHANGE BY ISAAC 4/17/08
				//curr_node = (k*r*c)+(i*c)+j; 
				curr_node = (k*r*c)+(j*c)+i;

				int intst = (int) IM[curr_node];	//Changed 5/9/08


				//Added by Yousef on jan 17, 2008
				//check if this is a seed point
				        
					Df = -log(F_H[(int)intst]);  //it was multiplied by .5                              
					if(Df>1000.0)
						Df = 1000;
					Db = -log(B_H[(int)intst]);                    
					if(Db>1000.0)
						Db=1000;     			
			        				

				g -> add_node();
				
				g -> add_tweights( curr_node,   /* capacities */ Df,Db);                         
			}
		}
	}

	//std::cout << "First Loop Complete" << std::endl;
	
	sig = 30.0;
	w=10.0;
	for(int k=0; k<z; k++)
    {		
        for(int j=0; j<r; j++)
        {			
			for(int i=0; i<c; i++)
			{
				/*get the neighbor edges capacities and write them to a file.
				Now, each edge capacity between two neighbors p and q represent
				the penalty for discontinuety. Since I am using the difference in
				the intensities, I should take the inverse so that very similar
				objects should have a large discontinuety penalty between them*/

				if(i>=c-1 || j>=r-1 || k>=z-1)
					continue;

				//Try this: Just define 3 edges to the neigbors				
				curr_node = (k*r*c)+(j*c)+i;
				intst = (int) IM[curr_node];
				
				//1.
				nbr_node = (k*r*c)+(j*c)+(i+1);				
				//if(Seg_out[nbr_node] == Seg_out[nbr_node])
				//	Dn = 0;
				//else
				//{
					intst2 = (int) IM[nbr_node];
					Dn = w*exp(-pow((double)intst-intst2,2)/(2*pow(sig,2)));				
				//}
				g -> add_edge( curr_node, nbr_node,    /* capacities */  Dn, Dn );
				//2.				
				nbr_node = (k*r*c)+((j+1)*c)+i;
				//if(Seg_out[nbr_node] == Seg_out[nbr_node])
				//	Dn = 0;
				//else
				//{
					intst2 = (int) IM[nbr_node];
					Dn = w*exp(-pow((double)intst-intst2,2)/(2*pow(sig,2)));				
				//}												
				g -> add_edge( curr_node, nbr_node,    /* capacities */  Dn, Dn );
				//3.				
				nbr_node = ((k+1)*r*c)+(j*c)+i;
				//if(Seg_out[nbr_node] == Seg_out[nbr_node])
				//	Dn = 0;
				//else
				//{
					intst2 = (int) IM[nbr_node];
					Dn = w*exp(-pow((double)intst-intst2,2)/(2*pow(sig,2)));				
				//}												
				g -> add_edge( curr_node, nbr_node,    /* capacities */  Dn, Dn );
			}
		}
	}    
	
	//std::cout << "Second Loop Complete" << std::endl;
	
	//Compute the maximum flow:
	g->maxflow();		//Alex DO NOT REMOVE
	
	for(int k=0; k<z; k++)
    {		
        for(int j=0; j<r; j++)
        {			
			for(int i=0; i<c; i++)
			{
				if(g->what_segment((k*r*c)+(j*c)+i) == GraphType::SOURCE)
					Seg_out[(k*r*c)+(j*c)+i]=0;
				else
					Seg_out[(k*r*c)+(j*c)+i]=255;
			}
		}
	}

	delete g;
}

void CompMixPoss(unsigned char* img, float* alpha_B, float* alpha_A, float* P_I, int R, int C, int shiftDown)
{
	//Copmute the normalized histogram of the image
	double Hist[256];
	long int K[256];
	double R1, R2, R3, a, b, c,r;
	for(int i=0; i<256; i++)
	{
		Hist[i] = 0;		
	}

	for(int i=0; i<R; i++)
	{
		for(int j=0; j<C; j++)
		{
			//Hist[img[i][j]]=Hist[img[i][j]]+1.0;	//BY ISAAC 4/9/08
			Hist[img[i*C + j]]=Hist[img[i*C + j]]+1.0;
		}
	
	}

	//Normalize the histogram and compute the first 3 moments
	
	R1=R2=R3=0;
	for(int i=0; i<256; i++)
	{
		Hist[i] = (Hist[i]/(R*C));// + eps;
		//fprintf(fid,"%f\n",Hist[i]);
		K[i] = i;
		R1 += K[i]*Hist[i];
		R2 += K[i]*K[i]*Hist[i];
		R3 += K[i]*K[i]*K[i]*Hist[i];
	}
	
	//compute the coefficients (a,b,c) or the quadratic eqn: a*Th^2+b*Th+c=0
	
	a = (R1*R1)+R1-R2;
	b = (R1*R1)-(R1*R2)+(2*R1)-(3*R2)+R3;
	c = (R2*R2)-(R1*R1)+(R1*R2)-(R1*R3);
	
	//Now, alpha_B and alpha_F are the roots of the above eqn:
	r = sqrt((b*b)-(4*a*c));
	alpha_B[0] = (-b+r)/(2*a);
	alpha_A[0] = (-b-r)/(2*a); 	

	//finally, compute the weighting coef. P_I:
	P_I[0] = (R1-alpha_A[0])/(alpha_B[0]-alpha_A[0]);

	
	//Some times you need to shift the means down. The next two lines are optional
	if(shiftDown == 1)
	{
		alpha_A[0] = max(alpha_A[0]/2,(alpha_B[0]+alpha_A[0])/2);
		alpha_B[0] = alpha_B[0]/1.5;
	}
	
}

void CompMixPoss3D(unsigned char* img, float* alpha_B, float* alpha_A, float* P_I, int R, int C, int Z, int shiftDown)
{
	//Copmute the normalized histogram of the image
	double Hist[256];
	long int K[256];
	double R1, R2, R3, a, b, c,r;
	for(int i=0; i<256; i++)
	{
		Hist[i] = 0;		
	}

	for(int i=0; i<R; i++)
	{
		for(int j=0; j<C; j++)
		{
			for(int k=0; k<Z; k++)
			{
				//Edit by Isaac 4/14/08
				//Hist[img[i][j][k]]=Hist[img[i][j][k]]+1.0;
				//Hist[img[k][i][j]]=Hist[img[k][i][j]]+1.0;	again 5/9/08
				Hist[img[(k*R*C)+(i*C)+j]]=Hist[img[(k*R*C)+(i*C)+j]]+1.0;
			}
		}
	
	}

	//Normalize the histogram and compute the first 3 moments
	
	R1=R2=R3=0;
	for(int i=0; i<256; i++)
	{
		Hist[i] = (Hist[i]/(R*C*Z));// + eps;
		//fprintf(fid,"%f\n",Hist[i]);
		K[i] = i;
		R1 += K[i]*Hist[i];
		R2 += K[i]*K[i]*Hist[i];
		R3 += K[i]*K[i]*K[i]*Hist[i];
	}
	
	//compute the coefficients (a,b,c) or the quadratic eqn: a*Th^2+b*Th+c=0
	
	a = (R1*R1)+R1-R2;
	b = (R1*R1)-(R1*R2)+(2*R1)-(3*R2)+R3;
	c = (R2*R2)-(R1*R1)+(R1*R2)-(R1*R3);
	
	//Now, alpha_B and alpha_F are the roots of the above eqn:
	r = sqrt((b*b)-(4*a*c));
	alpha_B[0] = (-b+r)/(2*a);
	alpha_A[0] = (-b-r)/(2*a); 	

	//finally, compute the weighting coef. P_I:
	P_I[0] = (R1-alpha_A[0])/(alpha_B[0]-alpha_A[0]);

	
	//Some times you need to shift the means down. The next two lines are optional
	if(shiftDown == 1)
	{
		alpha_A[0] = max(alpha_A[0]/2,(alpha_B[0]+alpha_A[0])/2);
		alpha_B[0] = alpha_B[0]/1.5;
	}
	
}

void MinErrorThresholding(unsigned char* img, float* alpha_B, float* alpha_A, float* P_I, int R, int C, int Z, int shiftDown, unsigned short *imgOut)
{
	typedef  short  InputPixelType;
	typedef  short  OutputPixelType;

	typedef itk::Image< InputPixelType,  3 >   InputImageType;
	typedef itk::Image< OutputPixelType, 3 >   OutputImageType;

	typedef itk::MinErrorThresholdImageFilter<
               InputImageType, OutputImageType >  FilterType;
  	//Create an itk image
	InputImageType::Pointer im;
	im = InputImageType::New();
	InputImageType::PointType origin;
    origin[0] = 0.; 
    origin[1] = 0.;    
	//if(Z>1)
		origin[2] = 0.;    
    im->SetOrigin( origin );

    InputImageType::IndexType start;
    start[0] =   0;  // first index on X
    start[1] =   0;  // first index on Y 
	//if(Z > 1)
		start[2] =   0;  // first index on Z 
    InputImageType::SizeType  size;
    size[0]  = C;  // size along X
    size[1]  = R;  // size along Y
	//if(Z > 1)
		size[2]  = Z;  // size along Z

    InputImageType::RegionType region;
    region.SetSize( size );
    region.SetIndex( start );
    
    im->SetRegions( region );
    im->Allocate();
    im->FillBuffer(0);
	im->Update();
	
	short minv = 255;
	short maxv = 0;
	//copy the input image into the ITK image
	typedef itk::ImageRegionIteratorWithIndex< InputImageType > IteratorType;
	IteratorType iterator1(im,im->GetRequestedRegion());
	int lng = R*C*Z;
	for(int i=0; i<lng; i++)
	{
		short val = (short)img[i];
		if (val > maxv)
			maxv = val;
		if (val < minv)
			minv = val;

		iterator1.Set((short)img[i]);
		++iterator1;	
	}

	//binarize
	FilterType::Pointer filter = FilterType::New();
	filter->SetInput( im );
	filter->SetNumberOfHistogramBins (128);
	filter->Update();

	//get output
	alpha_B[0] = ((float) filter->GetAlphaLeft());
	alpha_A[0] =  (float)filter->GetAlphaRight();
	P_I[0] = (float)filter->GetPriorLeft();
	
	//Some times you need to shift the means down. The next two lines are optional
	if(shiftDown == 1)
	{
		alpha_A[0] = max(alpha_A[0]/2,(alpha_B[0]+alpha_A[0])/2);
		alpha_B[0] = alpha_B[0]/1.5;
	}
	IteratorType iterate2(filter->GetOutput(),filter->GetOutput()->GetRequestedRegion());	
	for(int i=0; i<lng; i++)
	{				
		imgOut[i] = (int)iterate2.Get();
		++iterate2;			
	}
}


void Seg_GC_Full_3D_Blocks(unsigned char* IM, int r, int c, int z, double alpha_F, double alpha_B, double P_I, unsigned short* Seg_out, int* imBlock)
{   
    int nbr_node;
    double Dn, sig, w;
    double F_H[256], B_H[256];
    unsigned long int num_nodes, num_edges;
	typedef Graph_B<short,short,short> GraphType;  
      
    //Set the number of edges and the number of nodes and open the files that
    //will be used to save the weights	
    num_nodes = (imBlock[1]-imBlock[0])*(imBlock[3]-imBlock[2])*(imBlock[5]-imBlock[4]);
    num_edges = (imBlock[1]-imBlock[0]-1)*(imBlock[3]-imBlock[2]-1)*(imBlock[5]-imBlock[4]-1)*3;   
        
    //Before entering the loop, compute the poisson probs    
	//#pragma omp parallel for
	for(int i=0; i<256; i++)
    {
		if(i>=alpha_F)
			F_H[i] = (1-P_I)*compute_poisson_prob((int)alpha_F,alpha_F);
		else
			F_H[i] = (1-P_I)*compute_poisson_prob(i,alpha_F);
		if(i<=alpha_B)
			B_H[i] = P_I*compute_poisson_prob(int(alpha_B),alpha_B);
		else
			B_H[i] = P_I*compute_poisson_prob(i,alpha_B);
    }
	//std::cerr << "Poisson Probabilities Computed" << std::endl;
    
	//Construct the graph
	GraphType *g = new GraphType(/*estimated # of nodes*/ num_nodes, /*estimated # of edges*/ num_edges); 
	

	//std::cerr << "Graph Memory Allocated" << std::endl;

	//Here is the main loop.. 
    //For each point, compute the terminal and neighbor edge weights
	
	//std::cout << imBlock[5] - imBlock[4] << " " << imBlock[3] - imBlock[2] << " " << imBlock[1] - imBlock[0] << std::endl;
	for(int k=imBlock[4]; k<imBlock[5]; k++)
    {		       
		/*if (omp_get_thread_num() == 0)
			std::cout << "k-loop iteration " << k << " created " << omp_get_num_threads() << " threads to run" << std::endl;*/
		for(int j=imBlock[2]; j<imBlock[3]; j++)
        {			
			for(int i=imBlock[0]; i<imBlock[1]; i++)
			{
				int IND = (i - imBlock[0] + (imBlock[1] - imBlock[0]) * (j - imBlock[2]) + (imBlock[1] - imBlock[0]) * (imBlock[3] - imBlock[2]) * (k - imBlock[4]));
				
				/*Get the terminal edges capacities and write them to a file
				These capacities represent penalties of assigning each point to
				either the fg or the bg. Now, I am using the distance of the point
				to the fg and bg. Then a short distance between a point and a class (fg or bg)
				means the penalty of the assignement is low and vice versa*/ 
								

				//unsigned long int index = mxCalcSingleSubscript(prhs[0], nsubs, subs);
				//intst = (mxGetPr(prhs[0]))[index];
				// CHANGE BY ISAAC 4/14/08
				
				//CHANGE BY ISAAC 4/17/08
				//curr_node = (k*r*c)+(i*c)+j; 

				int curr_node = (k*r*c)+(j*c)+i;

				double intst = (int) IM[curr_node];	//Changed 5/9/08


				//Added by Yousef on jan 17, 2008
				//check if this is a seed point

				
				//std::cout << intst << std::endl;
				double Df = -log(F_H[(int)intst]);  //it was multiplied by .5                              	
				if(Df>1000.0)
					Df = 1000;

				double Db = -log(B_H[(int)intst]);
				if(Db>1000.0)
					Db=1000;

				g -> add_node();
				g -> add_tweights(IND,   /* capacities */ Df,Db);
			}
		}
	}

	//std::cout << "First Loop Complete" << std::endl;
	
	sig = 50.0;
	w=10.0;
	

	for(int k=imBlock[4]; k<imBlock[5]; k++)
    {		
		for(int j=imBlock[2]; j<imBlock[3]; j++)
        {			
			//#pragma omp parallel for ordered
			for(int i=imBlock[0]; i<imBlock[1]; i++)
			{
				int IND = (i - imBlock[0] + (imBlock[1] - imBlock[0]) * (j - imBlock[2]) + (imBlock[1] - imBlock[0]) * (imBlock[3] - imBlock[2]) * (k - imBlock[4])); //needed for threading correctness
				
				/*get the neighbor edges capacities and write them to a file.
				Now, each edge capacity between two neighbors p and q represent
				the penalty for discontinuety. Since I am using the difference in
				the intensities, I should take the inverse so that very similar
				objects should have a large discontinuety penalty between them*/

				if(i>=imBlock[1]-1 || j>=imBlock[3]-1 || k>=imBlock[5]-1)
					continue;

				//Try this: Just define 3 edges to the neigbors				
				int curr_node = (k*r*c)+(j*c)+i;
				double intst = (int) IM[curr_node];
				
				//1.
				int nbr_node = (k*r*c)+(j*c)+(i+1);				

				double intst2 = (int) IM[nbr_node];
				double Dn = w*exp(-pow((double)intst-intst2,2)/(2*pow(sig,2)));				
				
				//2.				
				nbr_node = (k*r*c)+((j+1)*c)+i;

				intst2 = (int) IM[nbr_node];
				double Dn2 = w*exp(-pow((double)intst-intst2,2)/(2*pow(sig,2)));														
				
				//3.				
				nbr_node = ((k+1)*r*c)+(j*c)+i;

				intst2 = (int) IM[nbr_node];
				double Dn3 = w*exp(-pow((double)intst-intst2,2)/(2*pow(sig,2)));				
										
				
				g -> add_edge( IND, IND+1,    /* capacities */  Dn, Dn );
				g -> add_edge( IND, IND+(imBlock[1]-imBlock[0]),    /* capacities */  Dn2, Dn2 );
				g -> add_edge( IND, IND+(imBlock[1]-imBlock[0])*(imBlock[3]-imBlock[2]),    /* capacities */  Dn3, Dn3 );
			}
		}
	}    
	
	//std::cout << "Second Loop Complete" << std::endl;
	
	//Compute the maximum flow:
	g->maxflow();		//Alex DO NOT REMOVE
	
	//cout << imBlock[0] << " " << imBlock[1] << " " << imBlock[2] << " " << imBlock[3] << " " << imBlock[4] << " " << imBlock[5] << endl;

	//#pragma omp parallel for
	for(int k=imBlock[4]; k<imBlock[5]; k++)
    {		
        for(int j=imBlock[2]; j<imBlock[3]; j++)
        {			
			for(int i=imBlock[0]; i<imBlock[1]; i++)
			{
				int IND = (i - imBlock[0] + (imBlock[1] - imBlock[0]) * (j - imBlock[2]) + (imBlock[1] - imBlock[0]) * (imBlock[3] - imBlock[2]) * (k - imBlock[4])); //needed for threading correctness
				
				if(g->what_segment(IND) == GraphType::SOURCE)
					Seg_out[(k*r*c)+(j*c)+i]=0;
				else
					Seg_out[(k*r*c)+(j*c)+i]=255; //seems incorrect, if Seg_out is unsigned short* then max value should be 65535?
					//Seg_out[(k*r*c)+(j*c)+i]=65535;
			}
		}
	}
	
	//std::cout << "Third Loop Complete" << std::endl;

	delete g;
}

void threeLevelMinErrorThresh(unsigned char* im, float* Alpha1, float* Alpha2, float* Alpha3, float* P_I1, float* P_I2, int r, int c, int z)
{
	//create a normalized image histogram
	float Hst[256];
	for(int i=0; i<256; i++)
		Hst[i] = 0.0;
	
	for(int i=0; i<r*c*z; i++)
	{
		int v = (int) im[i];
		Hst[v]++;
	}

	for(int i=0; i<256; i++)
		Hst[i] /= (r*c*z);


	//The three-level min error thresholding algorithm
	float P0, U0, P1, U1, P2, U2, U, J, min_J;
	min_J = 1000000.0;
	// Try this: we need to define a penalty term that depends on the number of parameters
	//The penalty term is given as 0.5*k*ln(n)
	//where k is the number of parameters of the model and n is the number of samples
	//In this case, k=6 and n=256
	double PenTerm3 =  sqrt(6.0)*log(256.0);
	for(int i=0; i<254; i++)//to set the first threshold
	{
		//compute the current parameters of the first component
		P0 = U0 = 0.0;		
		for(int l=0; l<=i; l++)
		{
			P0+=Hst[l];
			U0+=(l+1)*Hst[l];
		}
		U0 /= P0;

		for(int j=i+1; j<255; j++)//to set the second threshold
		{
			//compute the current parameters of the second component
			P1 = U1 = 0.0;		
		    for(int l=i+1; l<=j; l++)
			{
				P1+=Hst[l];
				U1+=(l+1)*Hst[l];
			}
			U1 /= P1;

			//compute the current parameters of the third component
			P2 = U2 = 0.0;		
		    for(int l=j+1; l<=255; l++)
			{
				P2+=Hst[l];
				U2+=(l+1)*Hst[l];
			}
			U2 /= P2;
			
			//compute the overall mean
			U = P0*U0 + P1*U1 + P2*U2;

			//Compute the current value of the error criterion function
			J =  U - (P0*(log(P0)+U0*log(U0))+ P1*(log(P1)+U1*log(U1)) + P2*(log(P2)+U2*log(U2)));
			//Add the penalty term
			J +=PenTerm3;
			
			if(J<min_J)
			{
				min_J = J;
				Alpha1[0] = U0;
				P_I1[0] = P0;
				Alpha2[0] = U1;
				P_I2[0] = P1;
				Alpha3[0] = U2;				
			}
		}
	}

	//try this: see if using two components is better
	//The penalty term is given as sqrt(k)*ln(n)	
	//In this case, k=4 and n=256
	double PenTerm2 =  2*log(256.0);
	for(int i=0; i<254; i++)//to set the first threshold
	{
		//compute the current parameters of the first component
		P0 = U0 = 0.0;		
		for(int l=0; l<=i; l++)
		{
			P0+=Hst[l];
			U0+=(l+1)*Hst[l];
		}
		U0 /= P0;

		for(int j=i+1; j<255; j++)//to set the second threshold
		{
			//compute the current parameters of the second component
			P1 = U1 = 0.0;		
		    for(int l=j; l<=255; l++)
			{
				P1+=Hst[l];
				U1+=(l+1)*Hst[l];
			}
			U1 /= P1;

			//compute the overall mean
			U = P0*U0 + P1*U1;

			//Compute the current value of the error criterion function
			J =  U - (P0*(log(P0)+U0*log(U0))+ P1*(log(P1)+U1*log(U1)));
			//Add the penalty term
			J +=PenTerm2;
			if(J<min_J)
			{
				min_J = J;
				Alpha1[0] = U0;
				P_I1[0] = P0;
				Alpha2[0] = U1;
				P_I2[0] = P1;
				Alpha3[0] = -1; //Just a negative number to let the program knows that two levels will be used		
			}
		}
	}
	
}

void Seg_GC_Full_2D_Three_Level(unsigned char* IM,
                    int r, 
                    int c, 
                    double alpha_F, 
                    double alpha_B1, 
					double alpha_B2,
                    double P_I1, 
					double P_I2,
                    int* num_nodes, 
                    int* num_edges, 
                    unsigned short* Seg_out)
{    
    int curr_node;
    int rght_node;
    int down_node;
    int diag_node;
    double Df;
    double Db;
    double Dr;
    double Dd; 
    double Dg; 
    double sig;
    double F_H[256];
    double B_H[256];
    typedef Graph_B<int,int,int> GraphType;
       
    //Set the number of edges and the number of nodes and open the files that
    //will be used to save the weights
    num_nodes[0] = r*c;
    num_edges[0] = 3*r*c-2*r-2*c+1;
        	
    
    //Before entering the loop, compute the poisson probs 
    for(int i=0; i<256; i++)
    {
		if(i>=alpha_F)
			F_H[i] = (1-P_I1-P_I2)*compute_poisson_prob((int)alpha_F,alpha_F);
		else
			F_H[i] = (1-P_I1-P_I2)*compute_poisson_prob(i,alpha_F);
		if(i<=alpha_B1)
			B_H[i] = (P_I1)*compute_poisson_prob(int(alpha_B1),alpha_B1)+(P_I2)*compute_poisson_prob(int(alpha_B2),alpha_B2);
		else
        B_H[i] = (P_I1)*compute_poisson_prob(i,alpha_B1)+(P_I2)*compute_poisson_prob(i,alpha_B2);
    }
	
    //Here is the main loop.. 
    //For each point, compute the terminal and neighbor edge weights
	GraphType *g = new GraphType(/*estimated # of nodes*/ num_nodes[0], /*estimated # of edges*/ num_edges[0]); 
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {			
			/*Get the terminal edges capacities and write them to a file
			These capacities represent penalties of assigning each point to
			either the fg or the bg. Now, I am using the distance of the point
			to the fg and bg. Then a short distance between a point and a class (fg or bg)
			means the penalty of the assignement is low and vice versa*/ 
								
		   //Isaac change on 5/9/08
		   //int intst = (int) IM[i][j];
			int intst = (int) IM[i*c + j];

            //Added by Yousef on jan 17, 2008
            //check if this is a seed point
            if(intst == 255)
            {
                Df = 0;
                Db = 1000;
            }
            else if(intst == 0)
            {
                Df = 1000;
                Db = 0;
            }
            else
            {                
                Df = -log(F_H[intst]);  //it was multiplied by .5          
                if(Df>1000.0)
                    Df = 1000;
                Db = -log(B_H[intst]);
                if(Db>1000.0)
                    Db=1000;
                
            }            
            
            curr_node = (i*c)+j; 
            
			g -> add_node();
			g -> add_tweights( curr_node,   /* capacities */ Df,Db);                            			
		}
	}

    sig = 25.0;
    for(int i=0; i<r-1; i++)
      {
      for(int j=0; j<c-1; j++)
        {			
        // get the neighbor edges capacities and write them to a file.
	// Now, each edge capacity between two neighbors p and q represent
	// the penalty for discontinuety. Since I am using the difference in
	// the intensities, I should take the inverse so that very similar
	// objects should have a large discontinuety penalty between them*/

	curr_node = (i*c)+j; 
        rght_node = curr_node+1;
        down_node = curr_node+c;
        diag_node = curr_node+c+1;
            
            
        //from Boykov's paper instead
	Dr = 10*exp(-pow((double)IM[i*c + j]-(double)IM[i*c + j+1],2)/(2*pow(sig,2)));
	g -> add_edge( curr_node, rght_node,    /* capacities */  Dr, Dr );	
            
                   
        //from Boykov's paper instead
Dd = 20*exp(-pow((double)IM[i*c + j]-(double)IM[(i+1)*c + j],2)/(2*pow(sig,2)));
g->add_edge( curr_node, down_node,    /* capacities */  Dd, Dd );
                 
Dg = 20*exp(-pow((double)IM[i*c + j]-(double)IM[(i+1)*c + (j+1)],2)/(2*pow(sig,2)));
g->add_edge( curr_node, diag_node,    /* capacities */  Dg, Dg );            
               
        }
      }    
	
    //Compute the maximum flow:
	g->maxflow();		//Alex DO NOT REMOVE


	int RR,CC;
	for(int i=0; i<num_nodes[0]; i++)
	{
		CC = ((long)i)%c;
        RR = (i-CC)/c;
		if(g->what_segment(i) == GraphType::SOURCE)
			Seg_out[RR*c + CC]=0;
		else
			Seg_out[RR*c + CC]=255;
	}

	delete g;
}

//void subtractGradientImage(unsigned char* IM, int r, int c, int z, int sampl_ratio)
//{
//	//create an ITK image
//	typedef    unsigned short     MyInputPixelType;
//	typedef itk::Image< MyInputPixelType,  3 >   MyInputImageType;
//	MyInputImageType::Pointer im;
//	im = MyInputImageType::New();
//	MyInputImageType::PointType origin;
//    origin[0] = 0.; 
//    origin[1] = 0.;    
//	origin[2] = 0.;    
//    im->SetOrigin( origin );
//
//    MyInputImageType::IndexType start;
//    start[0] =   0;  // first index on X
//    start[1] =   0;  // first index on Y    
//	start[2] =   0;  // first index on Z    
//    MyInputImageType::SizeType  size;
//    size[0]  = c;  // size along X
//    size[1]  = r;  // size along Y
//	size[2]  = z;  // size along Z
//  
//    MyInputImageType::RegionType region;
//    region.SetSize( size );
//    region.SetIndex( start );
//    
//    double spacing[3];
//	spacing[0] = 1; //spacing along x
//	spacing[1] = 1; //spacing along y
//	spacing[2] = sampl_ratio; //spacing along z
//
//    im->SetRegions( region );
//	im->SetSpacing(spacing);
//    im->Allocate();
//    im->FillBuffer(0);
//	im->Update();	
//
//	//copy the input image into the ITK image
//	typedef itk::ImageRegionIteratorWithIndex< MyInputImageType > IteratorType;
//	IteratorType iterator1(im,im->GetRequestedRegion());
//	for(int i=0; i<r*c*z; i++)
//	{
//		iterator1.Set((unsigned short) IM[i]);
//		iterator1++;
//	}
//	typedef itk::GradientMagnitudeImageFilter<MyInputImageType, MyInputImageType > FilterType;
//	FilterType::Pointer filter = FilterType::New();
//	filter->SetInput( im );
//	filter->Update();
//	IteratorType iterator2(filter->GetOutput(),filter->GetOutput()->GetRequestedRegion());	
//
//	//try to write that image for now
//	/*typedef itk::ImageFileWriter< MyInputImageType > WriterType;
//	WriterType::Pointer writer = WriterType::New();
//	writer->SetFileName("grad.tif");
//	writer->SetInput( filter->GetOutput() );
//	writer->Update();	*/
//
//	for(int i=0; i<r*c*z; i++)
//	{
//		unsigned char t = iterator2.Get();					
//		if(t>IM[i])
//			IM[i] = 0;
//		else
//			IM[i]-=t;	
//		iterator2++;
//	}	
//
//}

/*int post_binarization(int* bin_im, int kernel_size, int dim, int r, int c, int z)
{	  	

	int const dim2 = 3;
	typedef unsigned char                PixelType;
	//if(dim == 2)
	//	typedef itk::Image< PixelType, 2 > ImageType;
	//else
		typedef itk::Image< PixelType, dim2 > ImageType;

	//Create an itk image
	ImageType::Pointer im;
	im = ImageType::New();
	ImageType::PointType origin;
    origin[0] = 0.; 
    origin[1] = 0.;    
	if(dim == 3)
		origin[2] = 0.;    
    im->SetOrigin( origin );

    ImageType::IndexType start;
    start[0] =   0;  // first index on X
    start[1] =   0;  // first index on Y 
	if(dim == 3)
		start[2] =   0;  // first index on Z 
    ImageType::SizeType  size;
    size[0]  = c;  // size along X
    size[1]  = r;  // size along Y
	if(dim == 3)
		size[2]  = z;  // size along Z

    ImageType::RegionType region;
    region.SetSize( size );
    region.SetIndex( start );
    
    im->SetRegions( region );
    im->Allocate();
    im->FillBuffer(0);
	im->Update();
	
	//copy the input image into the ITK image
	typedef itk::ImageRegionIteratorWithIndex< ImageType > IteratorType;
	IteratorType iterator1(im,im->GetRequestedRegion());
	int lng = r*c*z;
	for(int i=0; i<lng; i++)
	{		
		iterator1.Set((unsigned char)bin_im[i]);
		++iterator1;	
	}
	
	//create the structuring elemet
	typedef itk::BinaryBallStructuringElement< PixelType, dim2 > KernelType;
	KernelType ball;
	KernelType ball2;
	KernelType::SizeType ballSize;
	KernelType::SizeType ballSize2;
	ballSize.Fill( kernel_size );
	ballSize2.Fill( kernel_size-1 );
	ball.SetRadius( ballSize );
	ball2.SetRadius( ballSize2 );
	ball.CreateStructuringElement();
	ball2.CreateStructuringElement();

	//start by opening
	typedef itk::BinaryMorphologicalOpeningImageFilter< ImageType, ImageType, KernelType > FilterType;
	FilterType::Pointer filter1 = FilterType::New();
	filter1->SetInput( im );
	filter1->SetKernel( ball );
	if( filter1->GetBackgroundValue() != 0 )
	{
		std::cerr << "Wrong Background default value" << std::endl;
		return EXIT_FAILURE;
    }
	filter1->SetBackgroundValue( 0 );

	if( filter1->GetForegroundValue() != 255 )
    {
		std::cerr << "Wrong Foreground default value" << std::endl;
		return EXIT_FAILURE;
	}
	filter1->SetForegroundValue( 255 ) ;
	
	//try
	//{
	//	filter1->Update() ;
	//}
	//catch( itk::ExceptionObject & err )
	//{
	//	std::cerr << "Error in performing binary morphological opening: " << err << endl ;
	//	return -1;
	//}

	//then closing
	typedef itk::BinaryMorphologicalClosingImageFilter< ImageType, ImageType, KernelType > FilterType2;
	FilterType2::Pointer filter2 = FilterType2::New();
	filter2->SetInput( filter1->GetOutput() );
	filter2->SetKernel( ball2 );
	 // test the default attribute values, and exercise the accesors
	if( !filter2->GetSafeBorder() )
	{
		std::cerr << "Wrong SafeBorder default value" << std::endl;
		return EXIT_FAILURE;
    }
	filter2->SafeBorderOff();
	filter2->SafeBorderOn();
	filter2->SetSafeBorder( 0 );
	if( filter2->GetForegroundValue() != 255 )
    {
		std::cerr << "Wrong Foreground default value" << std::endl;
		return EXIT_FAILURE;
    }
	filter2->SetForegroundValue( 255 );
	try
	{
		filter2->Update() ;
	}
	catch( itk::ExceptionObject & err )
	{
		std::cerr << "Error in performing binary morphological opening: " << err << endl ;
		return -1;
	}

	IteratorType iterate2(filter2->GetOutput(),filter2->GetOutput()->GetRequestedRegion());	
	for(int i=0; i<lng; i++)
	{		
		int ii = bin_im[i];
		bin_im[i] = (int)iterate2.Get();
		++iterate2;			
	}
	return 1;
}*/

