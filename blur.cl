__kernel void blur(uint w,uint h, __global uchar *data,__global uchar *out)
{
  uint size = w*h;
  
  uint i = get_global_id (0);
  /* 0 r, 1 g, 2 b */
  for (ushort sample = 0;sample<3;sample++){
    uint offset = sample*size;
    uint x = i%w;
    uint y = (i-x)/w;
  
    /*Bound checking*/
    uint res = 0;
    uint neighbours = 8;
    bool top=false,right=false,down=false,left=false;
    if (x>0) {
      res+=data [offset+i-1];
      left = true;
    }
    else neighbours--;
    if (x<w) {
      res+=data [offset+i+1];
      right = true;
    }
    else neighbours--;
    if (y>0) {
      res+=data [offset+i-w];
      top = true;
    }
    else neighbours--;
    if (y<h) {
      res+=data [offset+i+w];
      down = true;
    }
    else neighbours--;
    if (top&&right) res+=data [offset+i-w+1];
    else neighbours--;
    if (top&&left) res+=data [offset+i-w-1];
    else neighbours--;
    if (down&&right) res+=data [offset+i+w+1];
    else neighbours--;
    if (down&&left) res+=data [offset+i+w-1];
    else neighbours--;
    
    res/=neighbours;
  
    out [offset+i] = res;
  }
}
