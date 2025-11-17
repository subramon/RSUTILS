export  void
ispc_dist_j_k(
    uniform uint8 Xj[], 
    uniform uint8 Xk[], 
    uniform uint64 nD, // number of segments (as multiple of 64 bytes)
    uniform uint16 dout[] // output 
    )
{
  int status = 0;

  varying uint16 d = 0; 
  foreach ( i = 0 ... nD ) { 
    varying uint8 Xji = Xj[i];
    varying uint8 Xki = Xk[i];

    /*
    if ( Xji == Xki ) { d++; } 

    if ( Xji>>4 == Xki>>4 ) { d++; } 
    if ( Xji&0xF == Xki&0xF ) { d++; } 
    */

    varying uint8 topj = Xji >> 4; 
    varying uint8 topk = Xki >> 4; 
    d += (topj == topk) && (topj != 0);

    varying uint8 botj = Xji & 0xF;
    varying uint8 botk = Xki & 0xF;
    d += (botj == botk) && (botj != 0);
  }
  dout[0] =  reduce_add(d);
}
