#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


__global__ void MulMat(float* mat1 , int alto1, int ancho1 , float* mat2 , int alto2 , int ancho2 , float* matriz){
	
	int i,j,n,pos;
  float valor;

  //Calculo de la posicion en X y Y  

  int o = blockIdx.y*blockDim.y+threadIdx.y;
  int m = blockIdx.x*blockDim.x+threadIdx.x;

  //Condicion que me dice cuando operar

  if ( o < alto1 && m < ancho2 ){
      //posicion de escritura
      
      pos = o*ancho1+m;
      
      //posicion en X para la lectura de la matriz1 
      
      i = o*ancho1;

      //posicion en Y para la lectura de la matriz2 
      
      j=m;

      //valor a escribir y contador del ciclo 
      
      valor=0;
      n=0;
      //ciclo que hace el producto punto 
          while(n<alto2){
              valor=valor + mat1[i]*mat2[j];
              i= i+1;
              j=j+ancho2;
              n=n+1;
              }

      //escritura del valor en la matriz
      matriz[pos]=valor;
  }
}

int main(int argc , char *argv[] ){

  if (argc != 3){
    printf("Uso: MultMat FileName1 FileName2\n" );
  }else{
    FILE *archivo1 , *archivo2 , *archivof;

    int a1valor1, a1valor2 , a2valor1 , a2valor2 , tamanom1 , tamanom2 ,tamanom3;

    float *h_matriz1 , *h_matriz2, *h_matrizf,*d_matriz1 , *d_matriz2, *d_matrizf;

    //se abre el file :v
    archivo1 = fopen ( argv[1], "r" );

    // se lee el file
    fscanf (archivo1, "%d", &a1valor1);
    fscanf (archivo1, "%d", &a1valor2);

    // se calcula el tamaño

    tamanom1 = a1valor1*a1valor2;

    //se crea la matriz plana!! (chan chan chaaaaan)

    h_matriz1 = (float*)malloc(tamanom1*sizeof(float));

    // se llena la matriz plana

    int n;
    n=0;

    while(n<tamanom1){
      float numero;
      fscanf (archivo1, "%f,", &numero);
      h_matriz1[n] = numero;
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


    h_matriz2 = (float*)malloc(tamanom2*sizeof(float));

    // se llena la matriz plana

    n=0;

    while(n<tamanom2){

      float numero;
      fscanf (archivo2, "%f,", &numero);
      h_matriz2[n] = numero;
      n= n+1;
    }

    tamanom3 = a1valor1 * a2valor2;

  //espacio necesario para las matrices en el device     
    size_t SizeMat1 = tamanom1*sizeof(float);
    size_t SizeMat2 = tamanom2*sizeof(float);
    size_t SizeMat3 = tamanom3*sizeof(float);
 
  //reserva de memoria en el device 

    cudaMalloc(&d_matriz1, SizeMat1);
    cudaMalloc(&d_matriz2, SizeMat2);
    cudaMalloc(&d_matrizf, SizeMat3);
    h_matrizf = (float*)malloc(tamanom3*sizeof(float));

  //copia de vectores al device 

    cudaMemcpy( d_matriz1, h_matriz1, SizeMat1, cudaMemcpyHostToDevice);  
    cudaMemcpy( d_matriz2, h_matriz2, SizeMat2, cudaMemcpyHostToDevice);

  //calculo de la cantidad del tamaño del grid si cada bloque tiene 1024 hilos en X y Y 

    int blockSize , gridSizeX , gridSizeY ;

    //Numero de hilos por bloque 
    
    blockSize = 32; // 32*32 = 1024 donde  1024 es la cantidad de hilos por bloque! 

    //Numero de bloques en X y Y 

    gridSizeX = (int)ceil((float)a2valor2/blockSize);
    gridSizeY = (int)ceil((float)a1valor1/blockSize); 

    //creacion de las dimenciones para el kernel 

    dim3 dimBlock(blockSize,blockSize,1);
    dim3 dimGrid(gridSizeX,gridSizeY,1);

    //Cambiar por copia de matris final en dispositivo   
    MulMat<<<dimGrid,dimBlock>>>(d_matriz1,a1valor1,a1valor2,d_matriz2,a2valor1,a2valor2,d_matrizf);

    //Copiar resultado al host 
    cudaMemcpy( h_matrizf, d_matrizf,SizeMat3, cudaMemcpyDeviceToHost );


  //se cierran los archivoss de las matrices

	fclose(archivo1);
	fclose(archivo2);
 
  //se libera memoria en el host 

	free(h_matriz1);
  free(h_matriz2);

  //se libera memoria en el device 

  cudaFree(d_matriz1);
  cudaFree(d_matriz2);
  cudaFree(d_matrizf);

  //escritura en archivo de salida 

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
		
    fprintf(archivof , "%.0f," , h_matrizf[i]);
		i=i+1;
	}

  //se termina de liberar memoria

  free(h_matrizf);

  }

  return 0;
}
