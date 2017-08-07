#include <stdio.h>
#include <stdlib.h>


float *MulMat(float* mat1 , int alto1, int ancho1 , float* mat2 , int alto2 , int ancho2){
	float *matriz;
	
	matriz =(float*)malloc((alto1*ancho2)*sizeof(float));
	
	int i,j,o,m,n,cont,pos;
	
	float valor;
	
	pos = 0;
	o= 0;
	cont =0;
	
	while(o<alto1){
		m=0;
		while(m<ancho2){
			i = cont;
			j=m;
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
		cont = cont+ancho1;
		o=o+1;
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

	while(i<a1valor1){

		n=0;
		while(n<a2valor2){
			fprintf(archivof , "%f," , matrizf[n]);
			n=n+1;
		}

		fprintf(archivof, "\n");
		i=i+1;
	}

  }

  return 0;
}
