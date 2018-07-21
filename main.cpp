#include <iostream>
#include <string.h>
#include<cmath>
#include <fstream>

using namespace std;

///Obtiene la longitud de un número
static int obtenerLongitudNumero(long long numero){			
	return log10(numero) + 1;
}

///Obtiene la potencia en base diez.
///Se usa para obtener elprimer digito de un número.
static long long  obtenerPotenciaDeDiez(int potencia)
{
	return pow(10, potencia);
}

///Escribe en el archivo.
static void escribirEnArchivo(long long number, long long numeroOrdenado, ofstream& destino, long identificador)
{	
	destino << "Caso " << identificador << ": N=" << number << ", O=" << numeroOrdenado << endl;
	cout << "Contador: " << identificador << endl;
}

int main(int argc, char** argv) {
	
	///Se abre el archivo "entrada.txt"
	ifstream archivo("entrada.txt", ios::in);
	///Se crea el archivo salida.txt
	ofstream destino("salida.txt", ios::out);
    if(destino.fail())
    {		
       cerr << "Error al crear el archivo: salida.txt" << endl;
       return 0;
   }
       
	char linea[128];
    bool obtuvoCantidadCasos = false;    
    if(archivo.fail())
    {
		cerr << "Error al abrir el archivo entrada.txt" << endl;
	}
    else
    {	
    	//Se inicia el recorrido del archivo entrada.txt
	    while(!archivo.eof())
	    {	    	
	    	long cantidadNumeros = 0;
	    	archivo.getline(linea, sizeof(linea));
	        	   
			///obtiene la cantidad de casos a procesar leyendo el primer registro del archivo.
	        if(!obtuvoCantidadCasos)
	        {
	        	cantidadNumeros = stoll(linea);
	        	obtuvoCantidadCasos = true;
			}
			
			///Procesa siempre y cuando hallan mas de 0 cantida de casos.
			if(cantidadNumeros > 0)
			{
				///Procesa cada uno de los casos (Cada numero para saber el numero ordenado)
				for(int iteracion = 0; iteracion < cantidadNumeros; iteracion++)
				{
					archivo.getline(linea, sizeof(linea));
					long long number = stoll(linea);
					long long numeroOrdenado = 0;	
					int longitud = obtenerLongitudNumero(number);
					int vectorNumeroOrdenado[longitud];
					int longitudNumeroOrdenado = 1;	
					
					///Si el numero obtenido es de 1 solo digito, se devuelve el numero.
					if(longitud == 1)
					{	
						escribirEnArchivo(number, number, destino, iteracion+1);
						continue;
					}
					
					///Si el numero obtenido es de 2 digitos, se hace la comparación, si no cumple con el ordenamiento se realiza el calculo y lo escribe en el documento.
					if(longitud == 2)
					{
						int numeroUno = number/10;
						int numeroDos=number%10;
						if(numeroUno <= numeroDos)
						{
							numeroOrdenado = number;						
						}
						else
						{
							///Si el numero no es ordenado, al primer digito se le baja un numero y al segundo se le pondra 9.
							numeroUno = numeroUno -1;	
							numeroOrdenado = (numeroUno * 10)+9;							
						}
						
						escribirEnArchivo(number, numeroOrdenado, destino, iteracion+1);
						continue;
					}
					
					///Aca llegara si tiene 3 o más digitos.
					///La forma de calculo de los números uno a uno, es por potencias de 10, por ejemplo:
					///el numero 12345 para obtener el primer digito es 10 (elevado a la longitud-1): 10 elevado a 4 = 10000, al restarlo da : 
					///12345 / 10000 = 1. (y asi sicesivamente con cada numero), el resultante para el calculo del egundo digito sera:
					///12345 - (1*10000) = 2345 y se repiteel proceso.
					int i = 0;					
					long long potenciaDiezNumero = obtenerPotenciaDeDiez(longitud-1);
					bool romperCiclo=false;					
					int diferencia[longitud-1];
					long long numeroTemporal = number;
					int numeroInicial = number / potenciaDiezNumero;
					///Se haran las comparaciones de a 2 digitos, si su diferencia es mayor a 0 indicara que no es ascendente, 
					///uno a uno de forma consecutiva, es decir el numero 1215, tomara 2 digitos: 1 y 2, al restarlos = 1-2 = -1, por lo que se omite, se
					//toma el siguiente "1": en este caso compara 2 y 1 : 2-1 = 1, esto indica que no es ordenado, por lo que se procesa. 
					while (i < (longitud-1) && !romperCiclo)
					{	
						int j = i+1;							
						numeroTemporal = numeroTemporal - (numeroInicial*potenciaDiezNumero);
						potenciaDiezNumero = potenciaDiezNumero / 10;
						int numeroSecuencia = numeroTemporal / potenciaDiezNumero;				
						diferencia[i] = numeroInicial - numeroSecuencia;
						///Aca se esta validando si la diferencia es mayor a 0, para ser procesado
						if (diferencia[i] > 0)
						{
							///Se le baja uno al primer digito de la diferencia (ejemplo: 2 y 1 , al 2 le bajara a 1, (2-1)
							numeroInicial = numeroInicial - 1;
							///Hay un comportameinto especial cuando termina en 0 (porque se debe bajar a 9, se pierde un digito de los n que se tenian al principio)
							//, ejemplo 110, este debe dar 99, por lo que al primer digito que es 1, se le pone 9.
							if(numeroSecuencia == 0 && j == (longitud-1))
							{
								numeroInicial = 9;						
							}
							
							///Si ya se paso del primer digito de ser procesado, al cambiar un digito, se debe mirar hacia atras si los numeros deben ser disminuidos tambn,
							///eneste caso, a todas la diferencias que hacia atras dieron 0, ejemplo: 1221, cuando procesa el 3er y 4to digito, hace el cambio para el 3er digito,
							///quedando 1211, pero hacia atras el 2 debe ser cambiado tambien, y la diferencia entre 2-2 es 0, hace que tambien se coloque 1 (numeroInicial).
							if(i > 0)
							{
								int tempI = i-1;
								while(diferencia[tempI] == 0)
								{
									vectorNumeroOrdenado[tempI] = numeroInicial; 
									tempI = tempI -1;
								}
							}
							///Sele asigna al vector de digitos ordenados el numero calculado para el primner digito del calculo. 
							vectorNumeroOrdenado[i] = numeroInicial; 
							
							///Para eliminnar un digito cuando el valor es 110, se le asigna 0 al primero, eso hace que sea ignorado.
							if(numeroSecuencia == 0 && j == (longitud-1))
							{
								vectorNumeroOrdenado[0] = 0;
							}				
							
							///Al hacer el cambio del segundo digito, los demas digitos hacia atras deben ser llenados a 9, por lo que se recorre lo que queda de la 
							///longitud del vector y se asignan los 9.
							int tempLongitud = i+1;
							while(tempLongitud < longitud)
							{
								vectorNumeroOrdenado[tempLongitud] = 9; 
								tempLongitud ++;
								longitudNumeroOrdenado = longitudNumeroOrdenado + 1;
							}
							
							///Se rompe el ciclo, para que no siga recorriendo el ciclo.
							romperCiclo = true;	
							continue;					
						}
						
						///Asigna los valores ordenados que se van encontrando y mueve al segundo digito para que sea procesado con el numero consecutivo.
						vectorNumeroOrdenado[i] = numeroInicial;
						vectorNumeroOrdenado[j] = numeroSecuencia;
						longitudNumeroOrdenado = longitudNumeroOrdenado +1;	
						numeroInicial = numeroSecuencia;			
						++i;						
					}
								
					///Convierte el vector a valor string para que sea escrito en el archivo de salida.
					string str;
					for (long i = 0; i < longitudNumeroOrdenado; i++)
					{
						if(i == 0 && vectorNumeroOrdenado[i] == 0)
						{
							continue;
						}
						
						str += to_string(vectorNumeroOrdenado[i]);					
					}					
					escribirEnArchivo(number, stoll(str), destino, iteracion+1);
				}
			}			
	    }
	}
    
    ///Se cierran los documentos.
    destino.close();
	archivo.close();
	
	cout << "Termino";
	system("pause");		
	return 0;
}

