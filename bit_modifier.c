#include "errors.h"


void int_modifier_textsize(unsigned * ptextsize, FILE * f)
{
  unsigned new_textsize = 0;
  printf("The value of textsize is : %d\n",*ptextsize);
  printf("Enter the new value\n>");
  scanf("%d", &new_textsize);
  printf("Old value of textsize : %d. New one is : %d\n", *ptextsize, new_textsize);
  *ptextsize = new_textsize;

  while(fseek(f, 0, SEEK_SET) != 0)
    printf("fseek(f, 0, SEEK_SET)\n");
  //On écrit le int :
  fputc(new_textsize, f);
}

void int_modifier_datasize(unsigned * pdatasize, FILE * f)
{
  unsigned new_datasize = 0;
  printf("The value of datasize is : %d\n",*pdatasize);
  printf("Enter the new value\n>");
  scanf("%d", &new_datasize);
  printf("Old value of textsize : %d. New one is : %d\n", *pdatasize, new_datasize);
  *pdatasize = new_datasize;

  while(fseek(f, 4, SEEK_SET) != 0)
    printf("fseek(f, 4, SEEK_SET)\n");
  //On écrit le int :
  fputc(new_datasize, f);
}

void int_modifier_dataend(unsigned * pdataend, FILE * f)
{
  unsigned new_dataend = 0;
  printf("The value of new_dataend is : %d\n",*pdataend);
  printf("Enter the new value\n>");
  scanf("%d", &new_dataend);
  printf("Old value of textsize : %d. New one is : %d\n", *pdataend, new_dataend);
  *pdataend = new_dataend;

  while(fseek(f, 8, SEEK_SET) != 0)
    printf("fseek(f, 8, SEEK_SET)\n");
  //On écrit le int :
  fputc(new_dataend, f);
}


void ints_modifier(unsigned * ptextsize, unsigned * pdatasize, unsigned * pdataend, FILE * f)
{
  int choice = 0;
  do{
      printf("\nMake your choice, you want to modifys :\n"
	 "\t1 textsize,\n"
	 "\t2 datasize,\n"
	 "\t3 dataend.\n>"
  );
    scanf("%d", &choice);

  switch (choice)
  {
    case 1:
	    putchar('\n');
	    int_modifier_textsize(ptextsize, f);
	    break;
    case 2:
	    putchar('\n');
	    int_modifier_datasize(pdatasize, f);
	    break;
    case 3:
	    putchar('\n');
	    int_modifier_dataend(pdataend, f);
	    break;
    default:
	    break;
    }
  }while((choice > 3) || (choice <= 0));
}

void text_modifier(unsigned * ptextsize)
{
  /*
  printf("Instruction text[] = {\n");
  for(int i = 0 ; i < *ptextsize ; i++)
  {
    printf("\t0x%04x, ", text[i]);
    if (i % 4 == 3 || (i == (*ptextsize - 1)) )
      putchar('\n');
  }
  printf("};\n");
  printf("unsigned textsize = %d;\n", *ptextsize);
  */
}

void data_modifier()
{
    printf("Bienvenu dans data_modifier()\n");
}

void bit_modifier(FILE * f)
{
  while(fseek(f, 0, SEEK_END) != 0){};	//On se place à la fin du fichier
  int size_file = ftell(f);
  int bit_to_modify = 0;

  printf("\nWhat is the bit that you want to modify ?\n");
  do{
    printf("Enter a number between 0 and %d\n>",(size_file-4));
    scanf("%d", &bit_to_modify);
  }while((bit_to_modify > (size_file-4)) || (bit_to_modify < 0));

  //On va a la position demandée pour lire :
  while(fseek(f, (long)bit_to_modify, SEEK_SET) != 0)
    printf("fseek(f, (long)bit_to_modify, SEEK_SET)");
  //On lit le int a l'adresse :
  unsigned int_read = fgetc(f);
  unsigned new_value = 0;
  printf("bit to modify is the %dth. Its value is : 0x%x.\n", bit_to_modify, int_read);
  printf("Enter the new value\n>");
  scanf("%d", &new_value);

  //On va a la position demandée pour écrire :
  while(fseek(f, (long)bit_to_modify, SEEK_SET) != 0)
    printf("fseek(f, (long)bit_to_modify, SEEK_SET)\n");
  //On écrit le int :
  fputc(new_value, f);

  //On va a la position demandée pour lire :
  while(fseek(f, (long)bit_to_modify, SEEK_SET) != 0){};
  new_value = fgetc(f);
  printf("Old value : 0x%x. New value : 0x%x\n", int_read, new_value);}

void affichage(char * path_name, unsigned * ptextsize, unsigned * pdatasize, unsigned * pdataend)
{
  FILE * f = fopen(path_name, "r+");
  if (f == NULL)
    exit(-1);//Erreur d'ouverture du fichier binaire
  //else
  //printf("File sucessfully opened.\n");
    int bit_read = 0;
  while(fseek(f, 0, SEEK_END) != 0){};	//On se place à la fin du fichier
  int size_file = ftell(f);
  printf("The bin file has %d bits\n",size_file);
  if(fclose(f) != 0)
    exit(-2);

  //Ouverture du fichier :
  int handle = open(path_name,O_RDONLY);
  if(handle < 0)
    fprintf(stderr, "Erreur d'ouverture du fichier binaire");

  if( (bit_read = read(handle, ptextsize, sizeof(*ptextsize))) != sizeof(*ptextsize))
    fprintf(stderr, "Erreur de lecture de 'textsize'. %d bit lus au lieu de %d", bit_read, sizeof(*ptextsize));;

  if( (bit_read = read(handle, pdatasize, sizeof(*pdatasize))) != sizeof(*pdatasize))
    fprintf(stderr, "Erreur de lecture de 'datasize'. %d bit lus au lieu de %d", bit_read, sizeof(*pdatasize));;

  if( (bit_read = read(handle, pdataend, sizeof(*pdataend))) != sizeof(*pdataend))
    fprintf(stderr, "Erreur de lecture de 'dataend'. %d bit lus au lieu de %d", bit_read, sizeof(*pdataend));

  /*extern*/ Instruction * text = malloc(*ptextsize * sizeof(Instruction));
  //lecture des instructions :
  if( (bit_read = read(handle, text, *ptextsize * sizeof(Instruction))) != (*ptextsize * sizeof(Instruction)) )
    fprintf(stderr, "Erreur de lecture de 'text'. %d bit lus au lieu de %d", bit_read, (*ptextsize * sizeof(Instruction)));

  Word * data = malloc(*pdatasize * sizeof(Word));
  //lecture des données :
  if( (bit_read = read(handle, data, *pdatasize * sizeof(Word))) != (*pdatasize * sizeof(Word)))
    fprintf(stderr, "Erreur de lecture de 'data'. %d bit lus au lieu de %d", bit_read, (*pdatasize * sizeof(Word)));

  printf("Instruction text[] = {\n");
  for(int i = 0 ; i < *ptextsize ; i++)
  {
    printf("\t0x%04x, ", text[i]);
    if (i % 4 == 3 || (i == (*ptextsize - 1)) )
      putchar('\n');
  }
  printf("};\n");
  printf("unsigned textsize = %d;\n", *ptextsize);

  printf("\nWord data[] = {\n");
  //Affichage des données au format binaire:
  for(int i = 0 ; i < *pdatasize ; i++)
  {
    printf("\t0x%08x, ", data[i]);
    if (i % 4 == 3)
      putchar('\n');
  }
  printf("\n};\n");
  printf("unsigned datasize = %d;\n", *pdatasize);
  printf("unsigned dataend = %d;\n", *pdataend);

    //Fermeture du fichier :
  if(close(handle) != 0)
    fprintf(stderr, "Erreur de fermeture du fichier binaire dans <machine.c:read_program>");
}

void modification(char * path_name, unsigned * ptextsize, unsigned * pdatasize, unsigned * pdataend)
{
  FILE * f = fopen(path_name, "r+");
  if (f == NULL)
    exit(-1);//Erreur d'ouverture du fichier binaire
  //else
  //printf("File sucessfully opened.\n");
  int choice = 0;
  do{
    printf("\nMake your choice :\n"
	   "\t1 to modify textsize, datasize or dataend,\n"
	   "\t2 to modify text,\n"
	   "\t3 to modify data,\n"
	   "\t4 to any bit.\n>"
  );
    scanf("%d", &choice);

  switch (choice)
  {
    case 1:
	    ints_modifier(ptextsize, pdatasize, pdataend, f);
	    break;
    case 2:
	    text_modifier(ptextsize);
	    break;
    case 3:
	    data_modifier();
	    break;
    case 4:
	    bit_modifier(f);
	    break;
    default:
	    break;
    }
  }while((choice > 4) || (choice <= 0));
/*
  printf("\nWhat is the bit that you want to modify ?\n");
  do{
    printf("Enter a number between 0 and %d\n>",size_file);
    scanf("%d", &bit_to_modify);
  }while((bit_to_modify > size_file) || (bit_to_modify < 0));

  //On va a la position demandée pour lire :
  while(fseek(f, (long)bit_to_modify, SEEK_SET) != 0)
    printf("fseek(f, (long)bit_to_modify, SEEK_SET)");

  //On lit le int a l'adresse :
  int int_read = fgetc(f);
  int int_to_write = int_read+1;
  printf("bit to modify is the %dth. Its value is : 0x%x, we wil try to write : 0x%x\n", bit_to_modify, int_read, int_to_write);

  //On va a la position demandée pour écrire :
  while(fseek(f, (long)bit_to_modify, SEEK_SET) != 0)
    printf("fseek(f, (long)bit_to_modify, SEEK_SET)\n");
  //On écrit le int :
  fputc(int_to_write, f);

  //On va a la position demandée pour lire :
  while(fseek(f, (long)bit_to_modify, SEEK_SET) != 0){};
  int new_value = fgetc(f);
  printf("Old value : 0x%x. New value : 0x%x\n", int_read, new_value);
*/
  if(fclose(f) != 0)
    exit(-2);
}

int main(int argc, char *argv[])
{
  if (!argc == 2)
  {
    fprintf(stderr, "usage: '%s path_name' where path_name is the path to the bin file to modify.\n",argv[0]);
    exit(-3);
  }
  //else
  unsigned textsize;
  unsigned datasize;
  unsigned dataend;
  printf( "************************************************************\n"
	  "******************* Original file content ******************\n"
	  "************************************************************\n"
  );
  affichage(argv[1], &textsize, &datasize, &dataend);
  
  modification(argv[1], &textsize, &datasize, &dataend);
  
  printf( "\n************************************************************\n"
	  "********************* Final file content *******************\n"
	  "************************************************************\n"
  );
  affichage(argv[1], &textsize, &datasize, &dataend);

  putchar('\n');
  exit(0);
}