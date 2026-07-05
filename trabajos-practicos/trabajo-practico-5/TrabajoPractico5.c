// Sebastian Guerrero, Santiago Sinisterra y Federico Melo
// s.guerrero3@uniandes.edu.co, s.sinisterra@uniandes.edu.co y f.melo@uniandes.edu.co
// 202021249, 202022177 y 202021525

#include <stdio.h>

int empaquetar(int* fina, char* caden, int taman);

int main(){

    int tamano;
    char bool_invalid_size = 1;

    /* Pido al usuario el tamano del vector para la variable. Lo obligo a que sea mayor a 0. */
    while (bool_invalid_size) {
        printf("Por favor indique el tamano del vector:\n");
        scanf("%d", &tamano);
        if (tamano > 0) {  // Valido que n > 0.
            --bool_invalid_size;
        } else {
            printf("\nError: El tamano del vector debe ser mayor que 0. Por favor vuelva a intentarlo.\n\n");
        }
    }

    // Inicializar estructras
    int * final;
    char * cadena;

    final = (int*) calloc(tamano, sizeof(int));
    cadena = (char*) calloc(tamano*8+1, sizeof(char));

    // Pedir cadena a procesar. Se le pide al usuario hasta que el usuario escriba una cadena con caracteres correctos.
    char cadena_aceptada = 0;
    while (!cadena_aceptada) {

        /* Se inicializa la cadena con '0' */
        for (int i=0; i <= tamano*8+1; i++) {
            *(cadena+i) = '0';
        }

        printf("Por favor ingrese una cadena de maximo %d caracteres: \n", tamano*8);
        scanf("%s", cadena);

        /* Llama al empaquetador */
        __asm{
            push tamano  // Empujo parametros
            push cadena
            push final
            
            call empaquetar

            add esp, 12  // Quedo debajo de los parametros

            /* El procedimiento empaquetar retorna 1 si caracteres son correctos, 0 de lo contrario. Ese retorno queda en AL (por ser char).
            Lo movemos a cadena_aceptada para que el programa en C sepa si aceptar la cadena o pedirla de nuevo al usuario.
            Notese que esto en realidad no afecta ningun requisito del enunciado, solo facilita las pruebas y el uso del programa. */ 
            mov cadena_aceptada, al  // 
        }  

        if (!cadena_aceptada) {
            printf("\nError: La cadena ingresada contiene caracteres no aceptables. Por favor vuelva a intentarlo.\n\n");
        }
    }

    /* Se imprime el vector */
    for (int i = 0 ; i<tamano; i++){
        printf("v[%d] =   0x%08X\n", i, final[i]);
    }

    return 0;
}

int empaquetar(int* fina, char* caden, int taman){
    char cadena_aceptada;  // [EBP-1]
    __asm{

        mov [EBP-1], 1

        mov EAX, [EBP+8]  // EAX: Apuntador a vector de enteros.
        mov EBX, [EBP+12]  // EBX: Apuntador a cadena de caracteres.

        mov ESI, [EBP+16]  // ESI: Tamanno del vector de enteros
        imul ESI, 4  // ESI: Tamanno del vector de enteros por 4.

        add EAX, 4  //  El Byte 4 del vector de enteros es el inicio del primer entero por Big Endian 
        mov EDX, 5  //  Contador que cuenta 4, 3, 2, 1

        for:

            /*  Revisa si 'A' <= caracter <= 'F' */
            cmp [EBX], 'A'
            jl noEsMayuscula

            cmp [EBX], 'F'
            jg noEsMayuscula

                /* Si si, se convierte de hexa a decimal */
                add [EBX], 9
                and [EBX], 0fh 
                mov CL, [EBX]           
                jmp hecho

            noEsMayuscula:

            /*  Revisa si '0' <= caracter <= '9' */
            cmp [EBX], '0'
            jl noEsNumero

            cmp [EBX], '9'
            jg noEsNumero

                /* Si si es numero, a CL */
                and [EBX], 0fh
                mov CL, [EBX]
                jmp hecho 

            noEsNumero:  // Ni numero ni mayuscula

            cmp [EBX], 0
            jne noEsNull

                mov CL, 0
                jmp hecho

            noEsNull:  // Ni numero ni mayuscula ni null
                mov [EBP-1], 0  // No se acepta la cadena

            hecho:
                shl CL, 4  // Corrimiento no aritmetico 4 a la derecha. Queda abcd0000
                inc EBX  // Se pasa al siguiente caracter

            /* Lo mismo que antes, pero con el otro caracter de la pareja */

            cmp [EBX], 'A'
            jl noEsMayuscula1

            cmp [EBX], 'F'
            jg noEsMayuscula1

                add [EBX], 9
                and [EBX], 0fh
                mov CH, [EBX] 
                jmp hecho1

            noEsMayuscula1:

            cmp [EBX], '0'
            jl noEsNumero1

            cmp [EBX], '9'
            jg noEsNumero1

                and [EBX], 0fh
                mov CH, [EBX]
                jmp hecho1      

            noEsNumero1:  // Ni numero ni mayuscula

            cmp [EBX], 0
            jne noEsNull1

                mov CH, 0
                jmp hecho1

            noEsNull1:  // Ni numero ni mayuscula ni null
                mov [EBP-1], 0  // No se acepta la cadena

            hecho1:
                or CL, CH  // Or entre CL, que es abcd0000, y CH, que es 0000efgh, da abcdefgh.
                sub EAX, 1  // Se mueve al siguiente Byte del entero
                mov [EAX], CL  // Alli se almacena el byte abcdefgh
                inc EBX  // Se pasa al siguiente caracter

                sub EDX, 1  // Se resta primero, de forma que EDX es 4, 3, 2 o 1
                cmp EDX, 1
                jnz nocambio  // Si EDX es 1   
                    add EAX, 8  // Apuntar al siguiente entero del vector de enteros.
                    mov EDX, 5  // Reestablecer EDX

                nocambio:

        sub ESI, 1  // Se quita 1 al tamano del vector porque ya se empaco un byte
        cmp ESI, 0
        jnz for  // El for sigue hasta que el tamano sea 0.

        mov eax, 0
        mov al, [EBP-1]  // Retornar cadena_aceptada
    }
}

