#include <stdio.h>
#include <stdlib.h>

//NOMBRE: Federico Melo Barrero.
//CODIGO: 202021525.
//CORREO: f.melo@uniandes.edu.co

#define NOMBRES_SIZE 45
#define APELLIDOS_SIZE 45
#define CEDULA_SIZE 12
#define CODIGO_SIZE 4
#define NOTA_SIZE 2
#define ANYOSPENSION_SIZE 1
#define FILE_SIZE ( NOMBRES_SIZE + APELLIDOS_SIZE + CEDULA_SIZE + CODIGO_SIZE + NOTA_SIZE + ANYOSPENSION_SIZE )

//TO DO: Declare el tipo y tamaño adecuados para que las variables puedan contener el correspondiente valor
//del archivo de entrada
// NO MODIFIQUE NINGUNA OTRA COSA EN EL PROGRAMA
// Estructura para manejar los datos de entrada.
struct Datos {
   char nombres[45];
   char apellidos[45];
   char cedula[12];
   int codigo;
   short nota;
   char anyosPension;
} datos;

unsigned int getFileSize ( FILE * );
unsigned int loadData ( struct Datos *, char * );
unsigned int getNumber ( FILE *, char *, unsigned int );
void printChrs ( char *, unsigned int );

int main( int argc, char *argv[] )
{

   if ( argc != 2 ){
      printf( "*** ATENCION! Uso del comando:\n\tprograma archivo-de-datos\n" );
      return 1;
   }

   if ( loadData( &datos, argv[1] ) != FILE_SIZE ) return 1;

   printf( "Nombres:" );
   printChrs( datos.nombres, sizeof( datos.nombres ) );
   printf( "\n" );
   printf( "Apellidos:" );
   printChrs( datos.apellidos, sizeof( datos.apellidos ) );
   printf( "\n" );
   printf( "Cedula:" );
   printChrs( datos.cedula, sizeof( datos.cedula ) );
   printf( "\n" );
   printf( "Codigo:%u\n", datos.codigo );
   printf( "Nota:%u\n", datos.nota );
   printf( "Diferencia a pension:%d\n", datos.anyosPension);

   return 0;
} // main

/**
 * Calcula la longitud en bytes de un archivo.
 * Pre   El archivo esta abierto.
 * Parametros  f Apuntador al descriptor del archivo.
 * Retorna  la longitud en bytes del archivo apuntado por f.
 */
unsigned int getFileSize ( FILE * f ){

   unsigned int longArchivo;

   if ( fseek( f, 0, SEEK_END ) || ( longArchivo = ftell( f ) ) < 0 || fseek( f, 0, SEEK_SET ) ) {
      longArchivo = 0;
   }

  return longArchivo;
} // getFileSize

/**
 * Lee del archivo un número binario de l bytes en la variable apuntada por p.
 * El número se supone en formato big endian y lo convierte a little endian.
 */
unsigned int getNumber ( FILE * f, char * p, unsigned int l ){

   int i;

   p = p + l - 1;
   for ( i = 0; i < l && ( fread( p-i, sizeof( char ), 1, f ) == sizeof( char ) ); i++ );

   return i;
} // getNumber

/**
 * Inicializa una estructura con los datos del archivo.
 * Abre el archivo de datos, lo carga en memoria y los guarda en una estructura de datos.
 */
unsigned int loadData ( struct Datos * datosP, char * nombreArchivo ){

   FILE * f = NULL;
   unsigned int longFisica = 0;
   unsigned int nBytes;

   if ( !( f = fopen( nombreArchivo, "rb" ) ) ) {
      printf( "*** ATENCION! No se logro abrir el archivo %s\n", nombreArchivo );
   }
   else if ( getFileSize( f ) != FILE_SIZE ){
         printf( "*** ATENCION! Tamanio del archivo %s no concuerda\n", nombreArchivo );
   }
   else {	   
      nBytes = NOMBRES_SIZE;
      if ( fread( datosP->nombres, sizeof( char ), sizeof( datosP->nombres ), f ) != nBytes ) {
         printf( "*** ATENCION! No se pudo leer completos los nombres del archivo %s\n", nombreArchivo );
         return longFisica;
      }
      longFisica += nBytes;
      nBytes = APELLIDOS_SIZE;
      if ( fread( datosP->apellidos, sizeof( char ), sizeof( datosP->apellidos ), f ) != nBytes ) {
         printf( "*** ATENCION! No se pudo leer completos los apellidos del archivo %s\n", nombreArchivo );
         return longFisica;
      }
	  longFisica += nBytes;
      nBytes = CEDULA_SIZE;
      if ( fread( datosP->cedula, sizeof( char ), sizeof( datosP->cedula ), f ) != nBytes ) {
         printf( "*** ATENCION! No se pudo leer completa la cedula del archivo %s\n", nombreArchivo );
         return longFisica;
      }
	  longFisica += nBytes;
      nBytes = CODIGO_SIZE;
      if ( getNumber( f, (char *)(&datosP->codigo), sizeof( datosP->codigo ) ) != nBytes ) {
         printf( "*** ATENCION! No se pudo leer completo el codigo del archivo %s\n", nombreArchivo );
         return longFisica;
      }
	  longFisica += nBytes;
      nBytes = NOTA_SIZE;
      if ( getNumber( f, (char *)(&datosP->nota), sizeof( datosP->nota ) ) != nBytes ) {
         printf( "*** ATENCION! No se pudo leer la completa nota del archivo %s\n", nombreArchivo );
         return longFisica;
      }
	  longFisica += nBytes;
      nBytes = ANYOSPENSION_SIZE;
      if ( getNumber( f, (char *)(&datosP->anyosPension), sizeof( datosP->anyosPension ) ) != nBytes ) {
         printf( "*** ATENCION! No se pudo leer la completa diferencia a pension %s\n", nombreArchivo );
         return longFisica;
      }
      longFisica += nBytes;
   }

   fclose( f );

   return longFisica;
} // loadData

/**
 * Imprime l caracteres a partir del caracter apuntado por p.
 */
void printChrs ( char * p, unsigned int l ){

   int i;

   for ( i = 0; i < l; i++ ){
      printf( "%c", p[i] );
   }

} // printChrs

