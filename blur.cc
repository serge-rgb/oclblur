#include "ocl/ocl.h"
#include "tgaLoader/TgaLoader.h"
#include <CImg.h>
using namespace cimg_library;

int main(int argc, char *argv[])
{
  OCL ocl;
  int clerr;
  
  CImg<unsigned char> img ("tobias_funke.tga");
  CImg<unsigned char> res (img.width (),img.height (),1,3,0);
  
  unsigned int size = img.size ();
  unsigned int width = img.width ();
  unsigned int height = img.height ();
  unsigned char* texels = img.data();
  unsigned char* resulttexels = new unsigned char [size];

  resulttexels = new unsigned char [size];
  
  
  //Create buffer
  
  //source image texels
  cl_mem tex_buffer = clCreateBuffer(ocl.context,CL_MEM_READ_ONLY,
				     size,
				     texels,&clerr);
  CL_CHK (clerr);

  //destination image texels
  cl_mem res_buffer = clCreateBuffer(ocl.context,CL_MEM_WRITE_ONLY,
				     size,
				     res.data(),&clerr);
  CL_CHK (clerr);
  
  
  cl_command_queue q = clCreateCommandQueue (ocl.context,
					     ocl.devices [0],
					     0,
					     &clerr);
  CL_CHK (clerr);
  
  
  clerr = clEnqueueWriteBuffer (q,
				tex_buffer,
				CL_TRUE,
				0,
				img.size (),
				texels,
				0,NULL,NULL);
  CL_CHK (clerr);

  
  cl_program prgrm = ocl.createProgram("blur.cl");
  ocl.buildProgram (prgrm);
  cl_kernel kernel = ocl.createKernel (prgrm,"blur");
  clerr = clSetKernelArg (kernel,0,sizeof (unsigned int),&width);
  clerr = clSetKernelArg (kernel,0,sizeof (unsigned int),&height);
  clerr = clSetKernelArg (kernel,2,sizeof (cl_mem),(void*)&tex_buffer);
  CL_CHK (clerr);
  clerr = clSetKernelArg (kernel,3,sizeof (cl_mem),(void*)&res_buffer);
  CL_CHK (clerr);

  

  size_t global_wsize = size;
  size_t local_wsize = 32*4;
  
  clerr = clEnqueueNDRangeKernel (q,kernel,
				  1,//work dim
				  NULL,//offset
				  &global_wsize,
				  &local_wsize,
				  0,NULL,NULL);
  
  clEnqueueReadBuffer (q,
			res_buffer,
			CL_TRUE,0,
			size,
		       res.data(),
		       0,NULL,NULL);

  
  res.display ("result");
  delete [] resulttexels;
  return 0;
}
