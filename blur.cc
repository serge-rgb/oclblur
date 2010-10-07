#include "ocl/ocl.h"
#include "tgaLoader/TgaLoader.h"

int main(int argc, char *argv[])
{
  OCL ocl;
  TgaLoader tga;
  int clerr;
  TGA_BYTE *result;
  
  tga.load ("tobias_funke.tga");
  cout << "Size: "<<tga.tex->w << "x" << tga.tex->h << endl;
  
  //Create buffer
  

  //source image texels
  cl_mem tex_buffer = clCreateBuffer(ocl.context,CL_MEM_READ_ONLY,
				     tga.tex->w*tga.tex->h*sizeof (*tga.tex->texels)*tga.tex->bpp/8,
				     tga.tex->texels,&clerr);
  //destination image texels
  
  CL_CHK (clerr);

  
  
  
  return 0;
}
