#include <stdio.h>

// Nombre: Federico Melo Barrero.
// Codigo: 202021525.
// Correo: f.melo@uniandes.edu.co

void empaquetar_cadena_en_vector(int*, int, char*);

int main(void) {
    int size_int_vector;  // Variable para almacenar el tamanho del vector de int.

    /* Pido al usuario el tamanho del vector para la variable */
    int bool_invalid_size = 1;
    while (bool_invalid_size) {
        printf("Por favor ingrese el tamanho del vector: ");
        scanf("%d", &size_int_vector);
        if (size_int_vector > 0) {  // Valido que n > 0.
            --bool_invalid_size;
        } else {
            printf("El tamanho del vector debe ser mayor que 0. Por favor vuelva a intentarlo.\n");
        }
    }

    /* Apuntador al vector de int creado con calloc.*/
    int *p_int_vect = (int*) calloc(size_int_vector, sizeof(int));

    /* Apuntador a la cadena de caracteres con calloc.*/
    int size_string = size_int_vector*8 + 1;
    char *p_char_vect = (char*) calloc(size_string, sizeof(char));

    /* Pido al usuario que teclee la cadena y la inicializo.*/
    printf("Por favor ingrese un numero hexadecimal de maximo %d caracteres: ", size_string - 1);
    scanf("%s", p_char_vect);

    /* Invoco el procedimiento que empaqueta la cadena en el vector.*/
    empaquetar_cadena_en_vector(p_int_vect, size_int_vector, p_char_vect);

    /* Imprimo el vector de int posicion por posicion en el formato deseado.*/
    for (int i = 0; i < size_int_vector; ++i) {
        int numero = *(p_int_vect+i);
        printf("v[%d] =      0x%08X\n", i, numero);  // %08X formatea un hexa con letras mayúsculas de 8 dígitos.
    }

    return 0;
}


void empaquetar_cadena_en_vector(int *p_int_vect, int size_int_vector, char *p_char_vect) {
    int posicion_vector_int = 0;
    for (int i = 0; i < size_int_vector*8; i+=2) {

        /* En cada iteracion del ciclo, se cogen dos caracteres y se convierten en un byte. */
        unsigned char caracter1 = *(p_char_vect+i);
        unsigned char caracter2 = *(p_char_vect+i+1);

        unsigned char byte = 0;  // Acá se almacenará el binario en un byte.
        unsigned char byte_temporal = 0;  // Variable auxiliar.

        if (caracter1 != '\0') {
            if (caracter1 >= 'a') {
                byte = caracter1 - 'a' + 10;  // Para que 'a' valga 10. Con eso 'b' = 11, 'c' = 12, etc. 
            } else if (caracter1 >= 'A') {
                byte = caracter1 - 'A' + 10;
            } else {
                byte = caracter1 - '0';
            } 
            /* En cualquier caso byte queda con la forma 0000abcd y representa el primer digito hexa. */
            byte <<= 4;  // Con esto, byte queda con forma abcd0000.
        }
        if (caracter2 != '\0') {
            if (caracter2 >= 'a') {
                byte_temporal = caracter2 - 'a' + 10;
            } else if (caracter2 >= 'A') {
                byte_temporal = caracter2 - 'A' + 10;
            } else {
                byte_temporal = caracter2 - '0'; 
            } 
            /* Análogo a arriba: byte_temporal queda con forma 0000efgh y representa el segundo digito hexa. */
            byte |= byte_temporal;  // Como OR con 0 conserva el otro operando, abcd0000 OR 0000efgh = abcdefgh.
        }

        /* Ahora, se empaqueta el byte en el vector.
           Como quiero empaquetar 4 bytes en cada posición del vector, 
           posicion_vector_int incrementa cada vez que se hacen cuatro iteraciones del ciclo. */
        if (((i/2) % 4 == 0) && (i != 0)) {
            ++posicion_vector_int;
        }

        unsigned int contador_hasta_4 = ((i/2) % 4) + 1; // Cuenta cuantas veces ha corrido el ciclo, pero cuenta de 1 a 4 y reinicia.
        unsigned int int_temporal = 0;
        if (contador_hasta_4 == 1) {  // Es el primer byte que se empaqueta en la posición posicion_vector_int.
            int_temporal = byte;
            int_temporal <<= 24;  // Lo corro al fondo. Como es el primer byte, lo corro 24.
            *(p_int_vect+posicion_vector_int) = int_temporal; // Uso la misma estrategia de emplear OR que usé con byte_temporal!
        } else if (contador_hasta_4 == 2) {  // Es el segundo byte empaquetado en dicha posición.
            int_temporal = byte;
            int_temporal <<= 16;  // Lo corro 16 por ser el segundo
            *(p_int_vect+posicion_vector_int) |= int_temporal;  
        } else if (contador_hasta_4 == 3) {
            int_temporal = byte;
            int_temporal <<= 8;  
            *(p_int_vect+posicion_vector_int) |= int_temporal;
        } else if (contador_hasta_4 == 4) {
            int_temporal = byte;
            *(p_int_vect+posicion_vector_int) |= int_temporal;
        }
    }

    return;
}