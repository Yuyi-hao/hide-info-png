
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h> // it has defination of uint8_t

// global array of PNg signature
#define PNG_SIG_SIZE 8
uint8_t PNG_SIG[] = {137, 80, 78, 71, 13, 10, 26, 10};

void read_byte(FILE *file, void *buff, size_t buff_size, const char *source_file, int source_line)
{
  /// @brief Read a fixed size of buffer from a file
  /// @param file
  /// @param buff
  /// @param buff_size
  size_t n = fread(buff, buff_size, 1, file); // fread(result store in , size of single element we try to read, the amount of each element you want to read, file pointer)  we can switch also between size of the element and number of the element
  // check ouput of fread
  if (n != 1)
  {
    if (ferror(file))
    {
      fprintf(stderr, "%s: %d ERROR: could not read PNG header: %s\n", source_file, source_line, strerror(errno));
      exit(1);
    }
    else if (feof(file))
    {
      fprintf(stderr, "%s: %d ERROR: could not read PNG header reached end of file.\n", source_file, source_line);
      exit(1);
    }
    else
    {
      assert(0 && "unreachable");
    }
  }
}

void write_byte(FILE *file, void *buff0, size_t buff_cap)
{
  size_t n = fwrite(buff0, buff_cap, 1, file);
  if (n != 1)
  {
    if (ferror(file))
    {
      fprintf(stderr, "ERROR: Could not write %zu bytes to file: %s\n", buff_cap, strerror(errno));
      exit(1);
    }
    else
    {
      assert(0 && "Unreachable");
    }
  }
}

void print_byte(uint8_t *buff, size_t buff_cap)
{
  // print the content of buffer
  for (size_t i = 0; i < buff_cap; i++)
  {
    printf("%u ", buff[i]);
  }
  printf("\n");
}

void reverse_byte(void *buff0, size_t buff_cap)
{
  /// @brief Reverse the byte by swapping elements from first half to second half of the array
  /// @param buff
  /// @param buff_cape
  uint8_t *buff = buff0;
  for (size_t i = 0; i < buff_cap / 2; i++)
  {
    uint8_t temp = buff[i];
    buff[i] = buff[buff_cap - i - 1];
    buff[buff_cap - i - 1] = temp;
  }
}

/* void inject_custom_chunk(FILE *file, void *buff0, size_t buff_cap, uint8_t chunk_type[4], uint32_t chunk_crc){ */
/*   // one chuck consist of four parts size of chunk, chunk type, chunk, crc(cyclic redundancy check) bytes */
/*   uint32_t chunk_size = buff_cap; */
/*   reverse_byte(&chunk_size, sizeof(chunk_size)); */
/*   write_byte(file, &chunk_size, sizeof(chunk_size)); */
/*   write_byte(file, &chunk_type, 4); */
/*   write_byte(file, buff0, buff_cap);/ */
/*   write_byte(file, &chunk_crc, sizeof(chunk_crc)); */
/* } */

void usage(FILE *file, char *program)
{
  fprintf(file, "Usages: %s <input.png> <output.png>\n", program);
}

#define CHUNK_BUFF_CAP (32 * 1024)
uint8_t chunk_buff[CHUNK_BUFF_CAP];

int main(int argc, char **argv)
{
  (void)argc;
  assert(*argv != NULL); // Checks that atleast program name is provided if it is not NULL then execute following statement otherwise no
  char *program = *argv++;
  if (*argv == NULL)
  {
    // check for the file is provided or not
    usage(stderr, program);
    fprintf(stderr, "ERROR: No input file is provided.\n");
    exit(1);
  }
  char *input_filepath = *argv++;

  if (*argv == NULL)
  {
    // check for the file is provided or not
    usage(stderr, program);
    fprintf(stderr, "ERROR: No output file is provided.\n");
    exit(1);
  }

  char *output_filepath = *argv++;
  printf("Inspected file is \"%s\"\n", input_filepath);
  printf("Inspected file is \"%s\"\n", output_filepath);

  // Open the file and read it as binary
  FILE *input_file = fopen(input_filepath, "rb");
  if (input_file == NULL)
  {
    fprintf(stderr, "ERROR: The file <%s> could not be opened: %s.\n", input_filepath, strerror(errno));
    exit(1);
  }
  FILE *output_file = fopen(output_filepath, "wb");
  if (output_file == NULL)
  {
    fprintf(stderr, "ERROR: The file <%s> could not be opened in write mode: %s.\n", input_filepath, strerror(errno));
    exit(1);
  }
  
  // read the signature
  uint8_t sig[PNG_SIG_SIZE]; // allocate 8 byte using 8 bit int if i m not wrong this should be the explanation of this line of code
  read_byte(input_file, sig, PNG_SIG_SIZE, __FILE__, __LINE__);
  write_byte(output_file, sig, PNG_SIG_SIZE);

  // compare png buff with actual value
  if (memcmp(sig, PNG_SIG, PNG_SIG_SIZE) != 0)
  {
    fprintf(stderr, "ERROR: file doest not appear to be a valid PNG image.\n");
    exit(1);
  }
  printf("File <%s> is a valid PNG image.\n", input_filepath);

  printf("Signature: ");
  print_byte(sig, PNG_SIG_SIZE);

  bool quit = false;

  // reading further bytes
  while (!quit)
  {

    uint32_t chunk_size;
    read_byte(input_file, &chunk_size, sizeof(chunk_size),  __FILE__, __LINE__);
    write_byte(output_file, &chunk_size, sizeof(chunk_size));

    reverse_byte(&chunk_size, sizeof(chunk_size));
    // print the content of buffer

    uint8_t chunk_type[4];
    read_byte(input_file, chunk_type, sizeof(chunk_type),  __FILE__, __LINE__);
    write_byte(output_file, chunk_type, sizeof(chunk_type));

    if (*(uint32_t *)chunk_type == 0x444E4549)
    {
      quit = true;
    }
#if 1
    size_t n = chunk_size;
    while (n > 0)
    {
      size_t m = n;
      if (m > CHUNK_BUFF_CAP)
      {
        m = CHUNK_BUFF_CAP;
      }
      read_byte(input_file, chunk_buff, m,  __FILE__, __LINE__);
      write_byte(output_file, chunk_buff, m);
      n -= m;
    }
#else
    //  skip the chunk  basically skip the data part
    if (fseek(input_file, chunk_size, SEEK_CUR) < 0)
    {
      fprintf(stderr, "ERROR: could not skip the chunk: %s\n", strerror(errno));
      exit(1);
    }
#endif
    uint32_t chunk_crc;
    read_byte(input_file, &chunk_crc, sizeof(chunk_crc),  __FILE__, __LINE__);
    write_byte(output_file, &chunk_crc, sizeof(chunk_crc));


    if(*(uint32_t*)chunk_type == 0x52444849){
      uint32_t injected_size = 3;
      reverse_byte(&injected_size, sizeof(injected_size));
      write_byte(output_file, &injected_size, sizeof(injected_size));
      reverse_byte(&injected_size, sizeof(injected_size));
      char *injected_type = "jaNe";
      uint32_t injected_crc = 0;
      write_byte(output_file, injected_type, 4);
      write_byte(output_file, "HEY", injected_size);
      write_byte(output_file, &injected_crc, sizeof(injected_crc));      
    }

    
    printf("Chunk Size: %u\n", chunk_size);
    printf("Chunk type[decimal value]: ");
    print_byte(chunk_type, sizeof(chunk_type));
    // print the decimal values to ascii values
    printf("Chunk type[ASCII values]: %.*s (0x%08X)\n", (int)sizeof(chunk_type), chunk_type,
           *(uint32_t *)chunk_type); // type punning used here

    printf("Chunk CRC: 0x%08X\n", chunk_crc);
  }

  
  fclose(input_file);
  fclose(output_file);

  // printf("👋 Hey, Jane!\n");

  return 0;
}

