export  void
ispc_dist_j_k(
    uniform uint8 Xj[], 
    uniform uint8 Xk[], 
    uniform int nD, // number of bytes allocated to a distance vector
    uniform uint16 dout[] // output 
    )
{
  int status = 0;

  varying uint16 d = 0; 
  foreach ( i = 0 ... nD ) { 
    d += (Xj[i] == Xk[i]);
  }
  dout[0] =  reduce_add(d);
}
