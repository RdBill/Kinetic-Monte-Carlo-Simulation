#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include <cmath>

float D = 10000000;
float F= 1;
float N;
int steps=1000;

FILE * debug = fopen("Output_debug.txt", "w+");
int const size = 3000;

int coverage = 0;
int monomer_density = 0;
int island_density = 0;
float monomer_density_arr[size+1];
float island_density_arr[size+1];

#define walker 3;
#define island 30;
int const L = 300;

float P_f;
float P_d;
int lattice[L][L];

void data();
void draw();
void print_walkers();

typedef struct {
	int x;
	int y;
}monomer;

monomer monomer_list[L*L];
int ipointa[L][L];

int ip[L],im[L];//this helps to make direction areas nbxa, nbya taking into account pbcs
int nbxa[L][4],nbya[L][4];//these are direction arrays for any point on the lattice


int monomer_ct = 0;


void create_lattice(){

	for(int i = 0; i<L-1 ; i++){
		for(int j = 0; j<L-1; j++){
			lattice[i][j] = 0;
		}
	}

	for(int i = 0; i<3000; i++){
		monomer_density_arr[i] = 0 ;
		island_density_arr[i] = 0;		
	}
}



void print_walkers(){

	for(int i=0; i< monomer_ct; i++){
		//fprintf(debug, "Walker %d, is, %d, %d \n", i, monomer_list[i].x, monomer_list[i].y);
		
	}//for loop
	
}

void add_walker(int x, int y){

	if((x>299) || (y > 299)){
		return;
	}
	
	//fprintf(debug, "monomer_ct %d, beginning of add_walker\n", monomer_ct);
	
	monomer_list[monomer_ct].x = x;
	monomer_list[monomer_ct].y = y;
	ipointa[x][y]=monomer_ct;
	//fprintf(debug, "x=%d, y=%d, ipointa=%d\n", x,y,monomer_ct);

	monomer_ct++;				

	//fprintf(debug, "monomer_ct %d, beginning of add_walker\n", monomer_ct);
	print_walkers();
	
	if((x>299) || (y>299)){
		while(true) ;
	}
	
}


int remove_walker(int x, int y){

	if((x>299) || (y > 299)){
		return 0;
	}

	int count = 0;
	//fprintf(debug, "monomer_ct %d, beginning of remove_walker \n", monomer_ct);
	int iw;
	iw=ipointa[x][y];
	//fprintf(debug, "iw=%d, x=%d, y=%d\n", iw, x, y);
	
	monomer_list[iw].x = monomer_list[monomer_ct-1].x ;
	monomer_list[iw].y = monomer_list[monomer_ct-1].y ;
	ipointa[monomer_list[monomer_ct-1].x][monomer_list[monomer_ct-1].y] = iw;

	monomer_ct--;
	//fprintf(debug, "monomer_ct %d, end of remove_walker\n", monomer_ct);

	if((x>299) || (y>299)){
		while(true) ;
	}

	return 0;
}

void deposition(){
	
	int i = (rand() % L); 
	int j = (rand() % L);
		
	lattice[i][j] += 1;

	if((i>299) || (j>299)){
		//printf("Deposition beyond lattice\n");
		while(true){
			;
		}
	}
	
	if(lattice[i][j] == 1){
		add_walker(i,j);
	}
	else{
	  if(lattice[i][j]==2) {
		remove_walker(i, j);
		island_density++;
	}
	}		

	print_walkers();
	coverage++;
}

void diffusion(){
	
	//fprintf(debug, "monomer_ct %d, beginning of diffusion\n", monomer_ct);
		
	int random = (rand() % monomer_ct) ;

	
	
	//printf("random = %d, monomer count = %d \n", random, monomer_ct);
	
	int i = monomer_list[random].x ;
	int j = monomer_list[random].y ;

	if((i>299) || (j > 299)){
		return;
	}

	//printf("Diffusion i=%d, j=%d\n", i, j);
		int hop = (rand() % 4);
		int ii,jj;
		//printf("In line 143\n");
		ii=nbxa[i][hop];
		jj=nbya[j][hop];
		//printf("In line 146\n");

			if(lattice[ii][jj] == 0){
			  add_walker(ii,jj);
			}
			else if (lattice[ii][jj] == 1){
				remove_walker(ii, jj);
				island_density++;
			}
			lattice[ii][jj]++;
	//printf("In line 156\n");
	
	lattice[i][j]--;
	remove_walker(i,j);
	print_walkers();
//	remove_walker(i, j);
	//fprintf(debug, "monomer_ct %d, end of diffusion\n", monomer_ct);

}
/*
void draw(){
    //Get a console handle
    HWND myconsole = GetConsoleWindow();
    //Get a handle to device context
    HDC mydc = GetDC(myconsole);

    //Choose any color
    COLORREF COLOR= RGB(255,255,255); 

    //Draw pixels
    for(int i = 0; i < L; i++){
		for(int j = 0; j <L; j++) {
			if(lattice[i][j] > 0){

				//ifdef _APPLE_ SetMacPixel( i, j, COLOR);
				//#elif _WIN32 SetPixel( mydc,i,j,COLOR );
				//#endif*

				SetPixel(mydc,i,j,COLOR);
			
			}
       }
	}
    
	ReleaseDC(myconsole, mydc);
    //cin.ignore();
    
}
*/

void output(){
	
	FILE *file = fopen("Output.txt", "a+");
	fprintf(file, "New Timestep\n");
	
	for(int i = 0; i < L; i++){
		for(int j=0; j<L; j++){
			fprintf(file, "%d\n", lattice[i][j]);			
		}//end for loop		
	}//end for loop	
	
	fclose(file);
}

//set periodic boundary condition//
void pbc(){
	
	//1st colm = last colm
	for(int i = 0; i<L-1; i++){
		lattice[i][L-1] = lattice[i][0];	
	}//for loop
	
	//1st row = last row
	for(int i = 0; i <L-1; i++){
		lattice[L-1][i] = lattice[0][i];
	}
	
}


int main(){
	
	FILE *debug_output = fopen("Debug.txt", "w");

	bool deposit = true;
	/*
	printf("Enter value for diffusion rate D: ");
	scanf(" %f", &D);

	printf("Enter value for deposition rate F: ");
	scanf(" %f", &F);

	printf("Enter total number of steps: ");
	scanf(" %d", &steps);

	*/

	FILE *file = fopen("Output.txt", "w");
	fclose(file);
	
	int deposition_ct=0;

	create_lattice();
	//sets inverse pointer array to -1 
	for(int i=0;i<L;i++){
	  for(int j=0;j<L;j++){
	    ipointa[i][j]=-1;
	  }}

	//initialize ip and im arrays to take care to pbc's easilyn
	for(int i=0;i<L;i++){
	  ip[i]=i+1;
	  im[i]=i-1;
	}
	ip[L-1]=0;
	im[0]=L-1;
	//initialize nbxa[L][4] and nbya[L][4] for the 4 directions
	for(int i=0;i<L;i++){
	  nbxa[i][0]=ip[i];
	  nbya[i][0]=i;
	  nbxa[i][1]=i;
	  nbya[i][1]=im[i];
	  nbxa[i][2]=im[i];
	  nbya[i][2]=i;
	  nbxa[i][3]=i;
	  nbya[i][3]=ip[i];
	}	  

	int cov_i = 0;
	int step=0;

	while(coverage < (L*L*0.3)){

		step++;
		
		//fprintf(debug_output, "value = %d, x=%d, y=%d\n", 381, monomer_list[381].x, monomer_list[381].y);

		//printf("value = %d, x=%d, y=%d\n", 381, monomer_list[381].x, monomer_list[381].y);

		float Rdep = F*(L^2);
		float Rdiff = monomer_ct * D;

		P_d = Rdep / (Rdep + Rdiff);
		P_f = 1 - P_d;

		float random = (rand() % 10000)/10000.0;
		//fprintf(debug, "P_d %f, random %f\n", P_d, random);

		//printf("P_d %f, random %f\n", P_d, random);

		//fprintf(debug, "monomer_ct %d\n", monomer_ct);
		
		
		if((random < P_d) /*&& (deposit)*/){
			
			deposition();
			//fprintf(debug, "Deposition \n");
			//printf("Deposition \n");
			deposition_ct++;
		}
		else{
			//printf("Carrying out diffusion\n");
			diffusion();
			//printf("Diffusion \n");
		}
		
		//pbc();
		//print_walkers();
		//output();		
		//draw();

		//printf("In line 305\n");
		double cov = ceil( ( (coverage*1.0) / (L*L*1.0) ) * 10000) ;
		//printf("cov %f\n", cov);
		
		cov_i = (int) (cov) ;
		//printf("cov_i %d\n", cov_i);
			
		//printf("In line 315\n");
		monomer_density_arr[cov_i] = (monomer_ct*1.0)/(L*L*1.0);
		//printf("mon_den %f\n", monomer_density_arr[cov_i]);

		//printf("Island Density %d\n", island_density);
		island_density_arr[cov_i] = (island_density*1.0)/(L*L*1.0); 
		//printf("island_den %f\n", island_density_arr[cov_i]);

		//printf("In Main\n\n");
		
	}
	
	fclose(debug_output);
	data();
}

void data(){

	FILE *output = fopen("data.txt", "w+");

	fprintf(output, "Coverage \t Monomer Density \t Island Density\n");

	for(int i = 3; i <= 3000; i=i+3){

		fprintf(output, "%f \t %f \t %f\n", ((i*1.0)/10000.0), monomer_density_arr[i], island_density_arr[i]);

	}	

	fclose(output);
}