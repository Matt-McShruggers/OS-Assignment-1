/*

============================================================================
============================================================================

  ____  _                 _           _                            ____   
 / ___|(_)_ __ ___  _   _| | __ _  __| | ___  _ __    ___ _ __    / ___|  _ 
 \___ \| | '_ ` _ \| | | | |/ _` |/ _` |/ _ \| '__|  / _ \ '_ \  | |     (_)
  ___) | | | | | | | |_| | | (_| | (_| | (_) | |    |  __/ | | | | |___   _ 
 |____/|_|_| |_| |_|\__,_|_|\__,_|\__,_|\___/|_|     \___|_| |_|  \____| (_)
  / ___(_) ___| | ___   | |__   /_/_ ___(_) ___ ___     __| | ___         
 | |   | |/ __| |/ _ \  | '_ \ / _` / __| |/ __/ _ \   / _` |/ _ \        
 | |___| | (__| | (_) | | |_) | (_| \__ \ | (_| (_) | | (_| |  __/        
  \____|_|\___|_|\___/  |_.__/ \__,_|___/_|\___\___/   \__,_|\___|        
 (_)_ __  ___| |_ _ __ _   _  ___ ___ ___(_) ___  _ __   ___  ___         
 | | '_ \/ __| __| '__| | | |/ __/ __/ __| |/ _ \| '_ \ / _ \/ __|        
 | | | | \__ \ |_| |  | |_| | (_| (_| (__| | (_) | | | |  __/\__ \        
 |_|_| |_|___/\__|_|   \__,_|\___\___\___|_|\___/|_| |_|\___||___/        
                                                                          
============================================================================
============================================================================

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MEMSIZE 512

typedef struct Simulator Simulator;

struct Simulator{
  int mainMemoryAddresses[MEMSIZE];
  int accumulator;
  char UC[20];
  char CIR[20];
  char MAR[5];
  int MDR;
  int is_paused; // 0 = False, 1 = True
  void (*SET) (Simulator*, char*, char*);
  void (*ADD) (Simulator*, char*, char*, char*);
  void (*SUB) (Simulator*, char*, char*, char*);
  void (*INC) (Simulator*, char*);
  void (*DEC) (Simulator*, char*);
  void (*MOV) (Simulator*, char*, char*);
  void (*LDR) (Simulator*, char*);
  void (*STR) (Simulator*, char*);
  void (*SHW) (Simulator*, char*);
  void (*PAUSE) (Simulator*);
};

void SET( Simulator* sim, char *memAddress, char *value ){
  char* decodedMemAddress = memAddress+1;
  int integerMemAddress = atoi(decodedMemAddress);
  int integerValue = atoi(value);
  sim->mainMemoryAddresses[integerMemAddress-1] = integerValue;
  return;
}

void ADD( Simulator* sim, char *memAddress1, char *memAddress2, char *memAddress3 ){
  char* decodedMemAddress1 = memAddress1+1;
  int integerMemAddress1 = atoi(decodedMemAddress1);
  char* decodedMemAddress2 = memAddress2+1;
  int integerMemAddress2 = atoi(decodedMemAddress2);
  char* decodedMemAddress3 = memAddress3+1;
  int integerMemAddress3 = atoi(decodedMemAddress3);
  
  // ADD D1 NULL NULL
  if (strcmp(memAddress2, "NULL") == 0 && strcmp(memAddress3, "NULL") == 0){
    sim->accumulator = sim->accumulator + sim->mainMemoryAddresses[integerMemAddress1-1];
    strcpy(sim->MAR, memAddress1);
    sim->MDR = sim->mainMemoryAddresses[integerMemAddress1-1];
  }
  // ADD D1 D3 NULL 
  else if (strcmp(memAddress3, "NULL") == 0){
    sim->accumulator = sim->mainMemoryAddresses[integerMemAddress1-1];
    sim->accumulator = sim->accumulator + sim->mainMemoryAddresses[integerMemAddress2-1];
    strcpy(sim->MAR, memAddress2);
    sim->MDR = sim->mainMemoryAddresses[integerMemAddress2-1];
  }
  // ADD D1 D3 D4
  else {
    sim->accumulator = sim->mainMemoryAddresses[integerMemAddress1-1];
    sim->accumulator = sim->accumulator + sim->mainMemoryAddresses[integerMemAddress2-1];
    sim->mainMemoryAddresses[integerMemAddress3-1] = sim->accumulator;
    strcpy(sim->MAR, memAddress3);
    sim->MDR = sim->mainMemoryAddresses[integerMemAddress3-1];
  }
  return;
}

void SUB( Simulator* sim, char *memAddress1, char *memAddress2, char *memAddress3 ){
  char* decodedMemAddress1 = memAddress1+1;
  int integerMemAddress1 = atoi(decodedMemAddress1);
  char* decodedMemAddress2 = memAddress2+1;
  int integerMemAddress2 = atoi(decodedMemAddress2);
  char* decodedMemAddress3 = memAddress3+1;
  int integerMemAddress3 = atoi(decodedMemAddress3);
  
  // SUB D1 NULL NULL
  if (strcmp(memAddress2, "NULL") == 0 && strcmp(memAddress3, "NULL") == 0){
    sim->accumulator = sim->accumulator - sim->mainMemoryAddresses[integerMemAddress1-1];
    strcpy(sim->MAR, memAddress1);
    sim->MDR = sim->mainMemoryAddresses[integerMemAddress1-1];
  }
  // SUB D1 D2 NULL 
  else if (strcmp(memAddress3, "NULL") == 0){
    sim->accumulator = sim->mainMemoryAddresses[integerMemAddress1-1];
    sim->accumulator = sim->accumulator - sim->mainMemoryAddresses[integerMemAddress2-1];
    strcpy(sim->MAR, memAddress2);
    sim->MDR = sim->mainMemoryAddresses[integerMemAddress2-1];
  }
  // SUB D3 D2 D1
  else {
    sim->accumulator = sim->mainMemoryAddresses[integerMemAddress1-1];
    sim->accumulator = sim->accumulator - sim->mainMemoryAddresses[integerMemAddress2-1];
    sim->mainMemoryAddresses[integerMemAddress3-1] = sim->accumulator;
    strcpy(sim->MAR, memAddress3);
    sim->MDR = sim->mainMemoryAddresses[integerMemAddress3-1];
  }
  return;
}

void LDR( Simulator* sim, char *memAddress ){
  char* decodedMemAddress = memAddress+1;
  int integerMemAddress = atoi(decodedMemAddress);
  sim->accumulator = sim->mainMemoryAddresses[integerMemAddress-1];
  strcpy(sim->MAR, memAddress);
  sim->MDR = sim->mainMemoryAddresses[integerMemAddress-1];
  return;
}

void STR( Simulator* sim, char *memAddress ){
  char* decodedMemAddress = memAddress+1;
  int integerMemAddress = atoi(decodedMemAddress);
  sim->mainMemoryAddresses[integerMemAddress-1] = sim->accumulator;
  strcpy(sim->MAR, memAddress);
  sim->MDR = sim->mainMemoryAddresses[integerMemAddress-1];
  return;
}

void MOV( Simulator* sim, char *memAddress1, char *memAddress2 ){
  char* decodedMemAddress1 = memAddress1+1;
  int integerMemAddress1 = atoi(decodedMemAddress1);
  char* decodedMemAddress2 = memAddress2+1;
  int integerMemAddress2 = atoi(decodedMemAddress2);
  
  int valueToMove = sim->mainMemoryAddresses[integerMemAddress1-1];
  sim->mainMemoryAddresses[integerMemAddress2-1] = valueToMove;
  sim->mainMemoryAddresses[integerMemAddress1-1] = 0;
  return;
}

void INC( Simulator* sim, char *memAddress ){
  sim->LDR(sim, memAddress);
  sim->accumulator++;
  sim->STR(sim, memAddress);
  return;
}

void DEC( Simulator* sim, char *memAddress ){
  sim->LDR(sim, memAddress);
  sim->accumulator--;
  sim->STR(sim, memAddress);
  return;
}

void SHW_unpaused( Simulator* sim, char *memAddress ){
  if (memAddress[0] != "D"){
    return;
  }
  char* decodedMemAddress = memAddress+1;
  int integerMemAddress = atoi(decodedMemAddress);
  printf("%d\n", sim->mainMemoryAddresses[integerMemAddress-1]);
  return;
}

void SHW_paused( Simulator* sim, char *memAddress ){
  if (memAddress[0] == 'D'){
    char* decodedMemAddress = memAddress+1;
    int integerMemAddress = atoi(decodedMemAddress);
    printf("%d\n", sim->mainMemoryAddresses[integerMemAddress-1]);
  } else if (strcmp(memAddress, "ACC")==0){
    printf("%d\n", sim->accumulator);
  } else if (strcmp(memAddress, "MAR")==0){
    printf("%s\n", sim->MAR);
  } else if (strcmp(memAddress, "MDR")==0){
    printf("%d\n", sim->MDR);
  } else if (strcmp(memAddress, "CIR")==0){
    printf("%s\n", sim->CIR);
  } else if (strcmp(memAddress, "UC")==0){
    printf("%s\n", sim->UC);
  }
  return;
}

void SHW( Simulator* sim, char *memAddress ){
  if (sim->is_paused == 0){ // Si no está en pausa la ejecución, SHOW solo puede muestrar los valores guardados en direcciones de memoria
    SHW_unpaused(sim, memAddress);
  } else { // Si está pausado, SHOW puede mostrar cualquier dato (Sea de memoria o de registro en la CPU)
    SHW_paused(sim, memAddress);
  }
  return;
}

void PAUSE( Simulator* sim ){
  if (sim->is_paused != 0){ // Unpause
    sim->is_paused = 0;
  } else { // Pause
    sim->is_paused = 1;
  }
  return;
}

Simulator createSim(){
  Simulator sim;
  sim.accumulator = 0;
  sim.UC[0] = '\0';
  sim.CIR[0] = '\0';
  sim.MAR[0] = '\0';
  sim.MDR = 0;
  sim.is_paused = 0;
  sim.SET = &SET;
  sim.ADD = &ADD;
  sim.SUB = &SUB;
  sim.INC = &INC;
  sim.DEC = &DEC;
  sim.MOV = &MOV;
  sim.LDR = &LDR;
  sim.STR = &STR;
  sim.SHW = &SHW;
  sim.PAUSE = &PAUSE;
  return sim;
}

/*---------------------------------------------------------------------------------------------------------------------------------*/

// La función "split" fue sacada de: https://www.youtube.com/watch?v=Vp6OELK4gmo
char **split(char *string, char *seperators, int *count);

void control_unit(Simulator* sim, char **split_strings_arr, int *count_strings_parameter);

int main()
{
  // Crea la estructura con los registros necesarios
  Simulator s = createSim();

  // Intenta abrir el primer archivo de texto que contenga las lineas de codigo a leer por el simulador
  printf("\n<< Output del 1er programa: >>\n");
  FILE* file1 = fopen("asm1.txt", "r");
  if (!file1){
        printf("Error: No se pudo cargar el 1er programa\n");
        return 1;
  }
  char line1[500];
  
  // Lee linea por linea los contenidos
  while (fgets(line1, sizeof(line1), file1)){
    int count_strings1 = 0;
    char **split_strings1 = split(line1, " ", &count_strings1);
    split_strings1[3][strcspn(split_strings1[3], "\n")] = 0;

    // Manejar la ejecución de instrucciones mediante una función que simula la UC + un condicional que simula el END
    if (strcmp(split_strings1[0], "END") == 0){
      for (int i = 0; i < count_strings1; i++)
        free(split_strings1[i]);
      free(split_strings1);
      break;
    } else {
      control_unit(&s, split_strings1, &count_strings1);
    }

    // Liberar espacio alocado de memoria
    for (int i = 0; i < count_strings1; i++)
      free(split_strings1[i]);
    free(split_strings1);
  }
  fclose(file1);

  // Repetir el proceso hecho anteriormente para un 2do archivo de texto/"programa"
  printf("\n<< Output del 2do programa: >>\n");
  FILE* file2 = fopen("asm2.txt", "r");
  if (!file2){
        printf("Error: No se pudo cargar el 2do programa\n");
        return 1;
  }
  char line2[500];
  
  while (fgets(line2, sizeof(line2), file2)){
    int count_strings2 = 0;
    char **split_strings2 = split(line2, " ", &count_strings2);
    split_strings2[3][strcspn(split_strings2[3], "\n")] = 0;

    if (strcmp(split_strings2[0], "END") == 0){
      for (int i = 0; i < count_strings2; i++)
        free(split_strings2[i]);
      free(split_strings2);
      break;
    } else {
      control_unit(&s, split_strings2, &count_strings2);
    }

    for (int i = 0; i < count_strings2; i++)
      free(split_strings2[i]);
    free(split_strings2);
  }
  fclose(file2);

  // Repetir una 3ra vez para leer el ultimo archivo
  printf("\n<< Output del 3er programa: >>\n");
  FILE* file3 = fopen("asm3.txt", "r");
  if (!file3){
        printf("Error: No se pudo cargar el 3er programa\n");
        return 1;
  }
  char line3[500];
  
  while (fgets(line3, sizeof(line3), file3)){
    int count_strings = 0;
    char **split_strings = split(line3, " ", &count_strings);
    split_strings[3][strcspn(split_strings[3], "\n")] = 0;

    // END NULL NULL NULL
    if (strcmp(split_strings[0], "END") == 0){
      for (int i = 0; i < count_strings; i++)
        free(split_strings[i]);
      free(split_strings);
      break;
    } else {
      control_unit(&s, split_strings, &count_strings);
    }

    for (int i = 0; i < count_strings; i++)
      free(split_strings[i]);
    free(split_strings);
  }
  fclose(file3);

  printf("\n<< Fin de simulacion >>\n");
  return 0;
}

// Hace subdivisiones de un string dado, las cuales se hacen teniendo en cuenta los carácteres separadores (separators).
char **split(char *string, char *seperators, int *count)
{
  int len = strlen(string);
  *count = 0;
  int i = 0;
  while (i < len)
  {
    while (i < len)
    {
      if (strchr(seperators, string[i]) == NULL)
        break;
      i++;
    }
    
    int old_i = i;
    while (i < len)
    {
      if (strchr(seperators, string[i]) != NULL)
        break;
      i++;
    }
    if (i > old_i) *count = *count + 1;
  }
  
  char **strings = malloc(sizeof(char *) * *count);
  i = 0;

  char buffer[16384];
  int string_index = 0;
  while (i < len)
  {
    while (i < len)
    {
      if (strchr(seperators, string[i]) == NULL)
        break;
      i++;
    }
    
    int j = 0;
    while (i < len)
    {
      if (strchr(seperators, string[i]) != NULL)
        break;
      
      buffer[j] = string[i];
      i++;
      j++;
    }
    
    if (j > 0)
    {
      buffer[j] = '\0';

      int to_allocate = sizeof(char) *
                        (strlen(buffer) + 1);
      
      strings[string_index] = malloc(to_allocate);
      strcpy(strings[string_index], buffer);
      string_index++;
    }
  }
  return strings;
}

// Simulador de la Unidad de Control
void control_unit( Simulator* sim, char **split_strings_arr, int *count_strings_parameter ){
  // Guardando al CIR (Current Instruction Register) y UC
  if (strcmp(split_strings_arr[0], "SHW") != 0 && strcmp(split_strings_arr[0], "PAUSE") != 0 && sim->is_paused == 0){
    char current_instruction_string_part1[sizeof(split_strings_arr[0])];
    strcpy(current_instruction_string_part1, split_strings_arr[0]);
    char current_instruction_string_part2[sizeof(split_strings_arr[1])];
    strcpy(current_instruction_string_part2, split_strings_arr[1]);
    char current_instruction_string_part3[sizeof(split_strings_arr[2])];
    strcpy(current_instruction_string_part3, split_strings_arr[2]);
    char current_instruction_string_part4[sizeof(split_strings_arr[3])];
    strcpy(current_instruction_string_part4, split_strings_arr[3]);

    char current_instruction[sizeof(split_strings_arr[0])+3];
    sprintf(current_instruction, "%s %s %s %s", current_instruction_string_part1, current_instruction_string_part2, current_instruction_string_part3, current_instruction_string_part4);

    strcpy(sim->CIR, current_instruction);
    strcpy(sim->UC, current_instruction);
  }

  // Manejo de la leída de instrucciones en base a si la ejecución está pausada o no
  if (sim->is_paused == 0){
    if (strcmp(split_strings_arr[0], "SET") == 0){
      sim->SET(sim, split_strings_arr[1], split_strings_arr[2]);
    } else if (strcmp(split_strings_arr[0], "ADD") == 0){
      sim->ADD(sim, split_strings_arr[1], split_strings_arr[2], split_strings_arr[3]);
    } else if (strcmp(split_strings_arr[0], "SUB") == 0){
      sim->SUB(sim, split_strings_arr[1], split_strings_arr[2], split_strings_arr[3]);
    } else if (strcmp(split_strings_arr[0], "INC") == 0){
      sim->INC(sim, split_strings_arr[1]);
    } else if (strcmp(split_strings_arr[0], "DEC") == 0){
      sim->DEC(sim, split_strings_arr[1]);
    } else if (strcmp(split_strings_arr[0], "MOV") == 0){
      sim->MOV(sim, split_strings_arr[1], split_strings_arr[2]);
    } else if (strcmp(split_strings_arr[0], "SHW") == 0){
      sim->SHW(sim, split_strings_arr[1]);
    } else if (strcmp(split_strings_arr[0], "LDR") == 0){
      sim->LDR(sim, split_strings_arr[1]);
    } else if (strcmp(split_strings_arr[0], "STR") == 0){
      sim->STR(sim, split_strings_arr[1]);
    } else if (strcmp(split_strings_arr[0], "PAUSE") == 0){
      sim->PAUSE(sim);
    }
  } else if (sim->is_paused == 1){
    if (strcmp(split_strings_arr[0], "SHW") == 0){
      sim->SHW(sim, split_strings_arr[1]);
    } else if (strcmp(split_strings_arr[0], "PAUSE") == 0){
      sim->PAUSE(sim);
    }
  }

  return;
}