#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "main.h"
#include "stm32f1xx_hal.h"

extern void initialise_monitor_handles(void);

void LED_Init()
{
  LED_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

}

#define MAX_LINE_SIZE 1024

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
        if (c == '\n'){
            printf('n');
        }else if((c <= 20) || (c >= 127)){
            // printf(" %i", c);
            printf("/x%02x\n", c);
        }else{
            printf("%c", c);
        }
    } while (c != '\n' && c != EOF);
}


static char* readLine(void) {
    static char line[MAX_LINE_SIZE]; // Buffer for input
    
    if (fgets(line, sizeof(line), stdin)) {
        return line;
    }
    return NULL;
}

static char readCharacter(void){
    char cha;
    
    int discardCha = 0;
    // fflush(stdin);
    while(scanf("%c", &cha) != 1){
        // Discard extra characters in input buffer
        clean_stdin();
    }
    clean_stdin();
    return cha;
}


// Function to read an integer using semihosting
static int32_t readInteger(void){
    int32_t num = 0;

    int cha = 0;
    
    while(scanf("%"SCNi32, &num) != 1){//decimeal or hex
        printf("Not a integer, try again:\n");    
        clean_stdin(); //discard characters after the number
    }
    clean_stdin(); //discard possible newline
        
    return num;
}

// Function to read flaots using semihosting
static float readNumber(void){
    float num = 0;
    
    int cha = 0;
    while(scanf("%f", &num) != 1){//float
        printf("Not a number, try again:\n");
        clean_stdin(); //discard characters after the number
        
    }
    clean_stdin(); //discard possible newline
    
    return num;
}

int main() {
    initialise_monitor_handles();

    HAL_Init();
    LED_Init();

    // for(int i; i<3; i++) {
    //     HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
    //     printf("Hello world!\n");
    // }

    // printf("\nType something to the mirror:\n");
    // char *line = readLine();
    // printf("The line read: %s\n", line);

    char option = 0;
    do{	
        printf("\nChoose an option: \n");
        printf("i - read and display integer input \n");
        printf("f - read and display float input \n");
        printf("w - quit \n");

        option = readCharacter();
        switch (option){
            case 'i':
                printf("\nEnter integer value or hex: \n"); 
                int32_t num = readInteger();
                printf("Value is %i\n", num);
                break;
            case 'f':
                printf("\nEnter a value: \n");
                float numFloat = readNumber();
                printf("Value is %f\n", numFloat);
                break;
            case 'w':
                printf("Good bye!\n");

                NVIC_SystemReset();
            default:
                printf("Invalid option %c\n", option);
        }
    }while(option != 'a');
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}
