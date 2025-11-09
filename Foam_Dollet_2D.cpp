/*
compile
icpc -c Foam_Dollet_2D.cpp
icpc -c twister.cpp
icpc Foam_Dollet_2D.o twister.o -o Foam_Dollet_2D.exe

execute
./Foam_Dollet_2D.exe -N 900 -phi 1.2 -delta 0.1 -gamma 1.5 -w 1.0 -sigm_obs 5.0 -K 10.0 -fx_ext 0.005 -dt 0.1 -trajectory 1 -total_step 100000 -output_step 100 -run 1


*/
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sstream>
#include <time.h>
#include <sys/stat.h>
#include "twister.h"


const double Pi=3.141592653589793;

long n;
double L_x;
double L_y;
long list_max=500;//neighbor listに蓄える周りの粒子の最大数
double r_neighbor=3.0;
int **neighbor_list;
long *count_neighbor;
double *sigm;


void total_potential(double alpha, double *rx, double *ry, double *e, double *P, double *shear_stress);
void cal_force_neighbor(double alpha, double *rx, double *ry, double *fx, double *fy);
void update_neighbor_list(double *rx, double *ry);

int main(int argc, char *argv[]){

    long i,j;

  	long N;
	double alpha=2.5;//harmonic: alpha=2.0, Hertzian: alpha=2.5
	double phi;
	double delta;
	double gamma;
	double w;
	double sigm_obs;
	double K;
	double fx_ext;
	double dt;

	long run;
	long output_step;
	long total_step;
	long trajectory;


    for(i=1;i<argc;i++){
        if(!strcmp(argv[i],"-N")){////strcmp関数は文字列argv[i]と"-for"が一致したら0を返す
            sscanf(argv[++i],"%ld",&N);
        }
        else if(!strcmp(argv[i],"-alpha")){
            sscanf(argv[++i],"%lf",&alpha);
        }
        else if(!strcmp(argv[i],"-phi")){
            sscanf(argv[++i],"%lf",&phi);
        }
        else if(!strcmp(argv[i],"-delta")){
            sscanf(argv[++i],"%lf",&delta);
        }
        else if(!strcmp(argv[i],"-gamma")){
            sscanf(argv[++i],"%lf",&gamma);
        }
        else if(!strcmp(argv[i],"-w")){
            sscanf(argv[++i],"%lf",&w);
        }
        else if(!strcmp(argv[i],"-sigm_obs")){
            sscanf(argv[++i],"%lf",&sigm_obs);
        }
        else if(!strcmp(argv[i],"-K")){
            sscanf(argv[++i],"%lf",&K);
        }
        else if(!strcmp(argv[i],"-fx_ext")){
            sscanf(argv[++i],"%lf",&fx_ext);
        }
        else if(!strcmp(argv[i],"-dt")){
            sscanf(argv[++i],"%lf",&dt);
        }
        else if(!strcmp(argv[i],"-run")){
            sscanf(argv[++i],"%ld",&run);
        }
        else if(!strcmp(argv[i],"-r_neighbor")){
            sscanf(argv[++i],"%lf",&r_neighbor);
        }
        else if(!strcmp(argv[i],"-trajectory")){
            sscanf(argv[++i],"%ld",&trajectory);
        }
        else if(!strcmp(argv[i],"-output_step")){
            sscanf(argv[++i],"%ld",&output_step);
        }
        else if(!strcmp(argv[i],"-total_step")){
            sscanf(argv[++i],"%ld",&total_step);
        }
	}

	n=N;////nはグローバル変数で、function1_gradの中にも届く
	neighbor_list=(int**)malloc(N*sizeof(int*));///neighbor_list[N][list_max]
    for(i=0;i<N;i++){
        neighbor_list[i] = (int*)malloc(list_max*sizeof(int));
    }
	count_neighbor=(long*)malloc(N*sizeof(long));
	sigm = (double*)malloc(N*sizeof(double));

    //long seed=1993;
    //init_genrand(seed);
    init_genrand((unsigned long)(time(NULL)+run));///時間を用いた乱数の初期化

	printf("output_step=%ld\n",output_step);


	double rx[N];
	double ry[N];
	double rx_before[N];
	double ry_before[N];
	double fx[N];
	double fy[N];


	char dir_name[256];	
	sprintf(dir_name,"Flow_N%ld_alpha%.3lf_phi%.3lf_delta%.3lf_gamma%.3lf_w%.3lf_sigm_obs%.3lf_K%.3lf_fx_ext%lf_dt%.3lf",N,alpha,phi,delta,gamma,w,sigm_obs,K,fx_ext,dt);

    if( mkdir(dir_name, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP |
              S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH)==0 ){
         printf("Mkdir successed\n");
    }
    else{
        printf("Mkdir failed\n");
    }



	



	double bar_sigm=0.0;
	for(i=0;i<N;i++){
		double temp1=genrand_real3();
		double temp2=genrand_real3();
		sigm[i] = 1.0 + delta*sqrt(-2.0*log(temp1))*cos(2*Pi*temp2);
		bar_sigm += sigm[i];
	}
	bar_sigm/=N;
	double s2=0.0;
	for(i=0;i<N;i++){
		s2 += (sigm[i]-bar_sigm)*(sigm[i]-bar_sigm);
	}
	s2/=N;

	double A = delta/sqrt(s2);
	double B = 1.0 - bar_sigm*delta/sqrt(s2);
	
	double sum2=0.0;
	for(i=0;i<N;i++){
		sigm[i] = A*sigm[i] + B;
		sum2 += sigm[i]*sigm[i];
	}
	
	L_y = w + sqrt( w*w + (Pi/(4.0*gamma))*( (1.0/phi)*sum2 + sigm_obs*sigm_obs ) );
	L_x = gamma*L_y;

	printf("L_x=%.10lf, L_y=%.10lf\n",L_x,L_y);

	for(i=0;i<N;i++){
		rx[i] = genrand_real3()*L_x;
      	while(rx[i]>L_x){
			rx[i]=rx[i]-L_x; 
		}
      	while(rx[i]<0.0){ 
			rx[i]=rx[i]+L_x; 
		}
		ry[i] = w + genrand_real3()*(L_y-2*w);
	}


	update_neighbor_list(rx,ry);
	for(i=0;i<N;i++){
		rx_before[i]=rx[i];
		ry_before[i]=ry[i];
	}	

	////////////////////////////////
	////以上で初期配置の完成////////
	////////////////////////////////

	double energy;
	double pressure;
	double shear_stress;

	long MDstep=0;
	char file_name[256];
	char file_name_trajectory[256];
	sprintf(file_name,"data_total_step%ld_output_step%ld_run%ld.txt",total_step,output_step,run);
	sprintf(file_name_trajectory,"trajectory_total_step%ld_output_step%ld_run%ld.txt",total_step,output_step,run);

	char path_name[256];
    strcpy(path_name,dir_name);
    strcat(path_name,"/");
    strcat(path_name,file_name);
    FILE *fp_stress_strain=fopen(path_name,"w");

	strcpy(path_name,dir_name);
	strcat(path_name,"/");
	strcat(path_name,file_name_trajectory);
	FILE *fp_trajectory=fopen(path_name,"w");

	if(trajectory==1){
		//////////初期配置の出力///////////////////// 
		fprintf(fp_trajectory,"%lf\t%ld\t%.10lf\t%.10lf\n",MDstep*dt,N,L_x,L_y);
		for(i=0;i<N;i++){
			fprintf(fp_trajectory,"%.16e %.16e %.16e ",rx[i],ry[i],0.0);///positions in PBC box
			fprintf(fp_trajectory,"%.16e ",sigm[i]);///粒子の直径
			fprintf(fp_trajectory,"\n");
		}

	}

	for(MDstep=0;MDstep<total_step;MDstep++){

		total_potential(alpha,rx,ry,&energy,&pressure,&shear_stress);
		if(MDstep%output_step==0){	
			fprintf(fp_stress_strain,"%.10e\t%.10e\t%.10e\t%.10e\n",MDstep*dt,energy,pressure,shear_stress);
			printf("%.10e\t%.10e\t%.10e\t%.10e\n",MDstep*dt,energy,pressure,shear_stress);
		}
		printf("MDstep=%ld\n",MDstep);
		printf("Total step=%ld\n",total_step);
		double dr_max=0.0;
		for(i=0;i<N;i++){
			double dx=(rx[i]-rx_before[i]);
			double dy=(ry[i]-ry_before[i]);
			if(dx>0.5*L_x){
			   	dx-=L_x;
			}else if(dx<-0.5*L_x){
				dx+=L_x;
			}
			if(dy>0.5*L_y){
			   	dy-=L_y;
			}else if(dy<-0.5*L_y){
				dy+=L_y;
			}
			double dr_i=sqrt(dx*dx + dy*dy);///二粒子間の距離
			if(dr_i>dr_max){
				dr_max=dr_i;
			}
		}
		double sigm_max=1.0;///r_cut*sigm_max;
		double dr_th=(r_neighbor-sigm_max)/2;
		if(dr_max>dr_th){
			printf("UPDATE: dr_max=%lf, dr_th=%lf\n",dr_max,dr_th);
			update_neighbor_list(rx,ry);
			for(i=0;i<N;i++){
				rx_before[i]=rx[i];
				ry_before[i]=ry[i];
			}	
		}



  		cal_force_neighbor(alpha,rx,ry,fx,fy);


    	for(i=0;i<N;i++){
					
			rx[i] += (fx[i] + fx_ext)*dt;
			ry[i] += fy[i]*dt;


			///Wall
			if( ry[i] < w ){
				double F_L_wall = K*( w - ry[i] );
				ry[i] += F_L_wall*dt;
			}
			if( (L_y-w) < ry[i] ){
				double F_R_wall = -K*( ry[i] - (L_y-w) );
				ry[i] += F_R_wall*dt;
			}


			///Obstacle
			double sigm_obs = 5.0;
			double r = sqrt( (rx[i]-L_x/2)*(rx[i]-L_x/2) + (ry[i]-L_y/2)*(ry[i]-L_y/2) );
			if( r < sigm_obs/2 ){
				double F_x_obs = -K*(r-sigm_obs/2)*(rx[i]-L_x/2)/r;
				double F_y_obs = -K*(r-sigm_obs/2)*(ry[i]-L_y/2)/r;
				rx[i] += F_x_obs*dt;
				ry[i] += F_y_obs*dt;
			}

      		while(rx[i]>L_x){
				rx[i]=rx[i]-L_x; 
			}
      		while(rx[i]<0.0){ 
				rx[i]=rx[i]+L_x; 
			}
      		while(ry[i]>L_y){ 
				ry[i]=ry[i]-L_y;
			}
      		while(ry[i]<0.0){ 
				ry[i]=ry[i]+L_y; 
			}

    	}

		if( (trajectory==1) && (MDstep%output_step==0) ){
			fprintf(fp_trajectory,"%lf\t%ld\t%.10lf\t%.10lf\n",MDstep*dt,N,L_x,L_y);
			for(i=0;i<N;i++){
				fprintf(fp_trajectory,"%.16e %.16e %.16e ",rx[i],ry[i],0.0);///positions in PBC box
				fprintf(fp_trajectory,"%.16e ",sigm[i]);///粒子の直径
				fprintf(fp_trajectory,"\n");
			}
		}



	}//step loop 
	fclose(fp_stress_strain);
	fclose(fp_trajectory);

	free(sigm);

    for(i=0;i<N;i++){
	    free(neighbor_list[i]);
    }
	free(neighbor_list);
	free(count_neighbor);
}


void update_neighbor_list(double *rx, double *ry){
	
	int i,j;

   	double dx, dy;
	double rij;
	for(i=0;i<n;i++){
		for(j=0;j<list_max;j++){///neighborじゃない部分は全て-1
			neighbor_list[i][j]=-1;
		}
	}
   	for(i=0;i<n;i++){
		count_neighbor[i]=0;
    	for(j=0;j<n;j++){
			if(j!=i){
				dx=(rx[i]-rx[j]);
				dy=(ry[i]-ry[j]);
				if(dx>0.5*L_x){  
		     		dx=dx-L_x;
				}
				if(dx<-0.5*L_x){ 
					dx=dx+L_x;
				}
				if(dy>0.5*L_y){
			    	dy=dy-L_y;
				}
				if(dy<-0.5*L_y){ 
					dy=dy+L_y;
				}
				rij =sqrt( dx*dx + dy*dy );///二粒子間の距離
				if(rij < r_neighbor){//neighborであることの条件
					neighbor_list[i][count_neighbor[i]]=j;
					count_neighbor[i]+=1;
				}
			}
     	}
	}

}
void total_potential(double alpha, double *rx, double *ry, double *e, double *P, double *shear_stress){

	int i,j,k;
   	double dx, dy;
	double rij;

    *e=0.0;//totalのポテンシャルエネルギー
	*P=0.0;
    *shear_stress=0.0;
	double part_pressure=0.0;
	double part_shear=0.0;

   	for(i=0;i<n;i++){
    	for(k=0;k<count_neighbor[i];k++){
			j=neighbor_list[i][k];
			dx=(rx[i]-rx[j]);
			dy=(ry[i]-ry[j]);
			if(dx>0.5*L_x){
		    	dx-=L_x;
			}else if(dx<-0.5*L_x){
				dx+=L_x;
			}
			if(dy>0.5*L_y){
		    	dy-=L_y;
			}else if(dy<-0.5*L_y){
				dy+=L_y;
			}
			rij =sqrt(dx*dx + dy*dy);///二粒子間の距離
			double sigm_ij=0.5*(sigm[i]+sigm[j]);

			if(rij<sigm_ij){
				*e += (1.0/alpha)*pow((1.0-rij/sigm_ij),alpha);
				part_shear += - (dx*dy/(rij*sigm_ij))*pow((1.0-rij/sigm_ij),alpha-1.0);
				part_pressure += (rij/sigm_ij)*pow((1.0-rij/sigm_ij),alpha-1.0);
			}
     	}
	}
	*e=*e/n/2;//ダブルカウントを考慮
	*shear_stress = part_shear/(L_x*L_y)/2;//ダブルカウントを考慮
	*P = part_pressure/(L_x*L_y)/2/2;//ダブルカウントを考慮
}

void cal_force_neighbor(double alpha, double *rx, double *ry, double *fx, double *fy){
////あるgammでの周期境界条件を考慮した力の計算

	long i,j,k;	

   	double dx, dy;
	double rij;
	double f=0.0;
   	for(i=0;i<n;i++){//i粒子に働く力
    	fx[i]=0.0;
		fy[i]=0.0;
   	}  	
   	for(i=0;i<n;i++){
    	for(k=0;k<count_neighbor[i];k++){
			j=neighbor_list[i][k];
			dx=(rx[i]-rx[j]);
			dy=(ry[i]-ry[j]);
			if(dx>0.5*L_x){
		    	dx-=L_x;
			}else if(dx<-0.5*L_x){
				dx+=L_x;
			}
			if(dy>0.5*L_y){
		    	dy-=L_y;
			}else if(dy<-0.5*L_y){
				dy+=L_y;
			}
			rij=sqrt(dx*dx + dy*dy);///二粒子間の距離
			double sigm_ij=0.5*(sigm[i]+sigm[j]);

			if(rij<sigm_ij) {
	  			f = pow((1.0-rij/sigm_ij),alpha-1.0)/(rij*sigm_ij);
	  			fx[i] += dx*f;
	  			fy[i] += dy*f;
			}

     	}
	}
}
