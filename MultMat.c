#include <stdio.h>
#include <stdlib.h>


float *MulMat(float* mat1 , int alto1, int ancho1 , float* mat2 , int alto2 , int ancho2){
	float *matriz;
	
	matriz =(float*)malloc((alto1*ancho2)*sizeof(float));
	
	int i,j,o,m,n,cont,pos,IDhilo,Nhilos,PosMAx;
	
	float valor;
	PosMAx = alto1*ancho2;
	pos = 0;
	o= 0;
	cont =0;
	
#pragma omp parallel private(pos,cont) shared(o) 
  	{ 
  	for (0; o<alto1; ++o){
		m=0;
		IDhilo = omp_get_thread_num();
		Nhilos = omp_get_num_threads();
		pos = pos + IDhilo*ancho1;
		cont = o*ancho1;
			while(m<ancho2){
				i = cont;
				j=m;
				printf("%d %d %d \n",m,Nhilos, cont);
				valor=0;
				n=0;
				while(n<alto2){
					valor=valor + mat1[i]*mat2[j];

					i= i+1;
					j=j+ancho2;
					n=n+1;
				}
			
				matriz[pos]=valor;
				pos= pos+1;
				m=m+1;
				}
  			}
  		
	} 	
	for (i = 0; i < PosMAx; ++i)
	{
		printf("%f\n",matriz[i]);
	}
	
	return matriz;
	 
}

int main(int argc , char *argv[] ){

  if (argc != 3){
    printf("Uso: MultMat FileName1 FileName2\n" );
  }else{
    FILE *archivo1 , *archivo2 , *archivof;

    int a1valor1, a1valor2 , a2valor1 , a2valor2 , tamanom1 , tamanom2;

    float *matriz1 , *matriz2, *matrizf;

    //se abre el file :v
    archivo1 = fopen ( argv[1], "r" );

    // se lee el file
    fscanf (archivo1, "%d", &a1valor1);
    fscanf (archivo1, "%d", &a1valor2);

    // se calcula el tamaño

    tamanom1 = a1valor1*a1valor2;

    //se crea la matriz plana!! (chan chan chaaaaan)

    matriz1 = (float*)malloc(tamanom1*sizeof(float));

    // se llena la matriz plana

    int n;
    n=0;

    while(n<tamanom1){
      float numero;
      fscanf (archivo1, "%f,", &numero);
      matriz1[n] = numero;
      n= n+1;
    }

    //se abre el file :v
    archivo2 = fopen ( argv[2], "r" );

    // se lee el file
    fscanf (archivo2, "%d", &a2valor1);
    fscanf (archivo2, "%d", &a2valor2);

    // se calcula el tamaño

    tamanom2 = a2valor1*a2valor2;

    //se crea la matriz plana!! (chan chan chaaaaan)

    matriz2 = (float*)malloc(tamanom2*sizeof(float));

    // se llena la matriz plana

    n=0;

    while(n<tamanom2){
  
      float numero;
      fscanf (archivo2, "%f,", &numero);
      matriz2[n] = numero;
      n= n+1;
    }

	matrizf = MulMat(matriz1,a1valor1,a1valor2,matriz2,a2valor1,a2valor2);
	
	fclose(archivo1);
	fclose(archivo2);
	
	free(matriz1);
  free(matriz2);
	
	archivof = fopen ( "out.txt", "w" );
	
	int tm;
	tm = a1valor1*a2valor2;
	
	fprintf(archivof , "%d\n%d\n" , a1valor1 , a2valor2 );

	int i;

	i=0;

	while(i<tm){

		if(i%a2valor2 == 0){
			fprintf(archivof , "\n");
		}
		fprintf(archivof , "%f," , matrizf[i]);
		i=i+1;
	}

  }

  return 0;
}
