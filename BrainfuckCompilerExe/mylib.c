#include "mylib.h"

void*
readfile_ex(
    void *dst,
    size_t dstsize,
    const char *fname,
    size_t offset, 
    size_t *psize,
    int binary
    )
{
  FILE *f;
  long fsize; 
  void *ptr = dst;

  if(binary)
    f = fopen(fname, "rb");
  else
    f = fopen(fname, "r");

  if(f == NULL)
  {
    printf("error: could not open file \"%s\"\n", fname);
    return NULL;
  }
  
  fseek(f, 0, SEEK_END);
  fsize = ftell(f) - offset;
  fseek(f, offset, SEEK_SET);

  if(!ptr)
  {
    ptr = malloc(fsize);
    if(ptr == NULL)
    {
      puts("error: not enough memory");
      return NULL;
    }
    dstsize = fsize;
  }

  fread(ptr, 1, dstsize, f);

  fclose(f);

  if(psize)
    *psize = (size_t)fsize;

  return ptr;    
}

void*
readfile(
    const char *fname,
    size_t *psize
    )
{
  return readfile_ex(
      NULL, 0, fname, 0, psize, 1
      );
}

void
histogram(
    void *src,
    size_t count,
    int *table
    )
{
  unsigned char *data = 
    (unsigned char*)src;

  size_t i;

  for(i = 0; i < count; i++)
  {
    table[data[i]]++;    
  }
}

void
writefile(
    const char *fname,
    void *data,
    size_t datasize
    )
{
  FILE *f = fopen(fname, "wb");
  fwrite(data, 1, datasize, f);
  fclose(f);
}



void
printhex(
    void *src,
    size_t size
    )
{
  unsigned char *data = src;
  size_t i, j;

  for(j = 0; j < size; j+=HEX_WIDTH)
  {
    printf("%.8x: ", j);

    size_t limit = HEX_WIDTH;
    if(j + HEX_WIDTH > size)
      limit = size - j;

    for(i = 0; i < HEX_WIDTH; i++)
    {
      if((i % 4) == 0 &&
         (i % HEX_WIDTH) != 0)
        putchar(' ');

      if(i < limit)
        printf("%.2x ", data[j+i]);
      else
        printf("   ");
    }

    putchar(' ');

    for(i = 0; i < limit; i++)
    {
      if(data[j+i] < ' ' ||
         data[j+i] > '~')
        putchar('.');
      else
        putchar(data[j+i]);
    }

    putchar('\n');  
  }

}

