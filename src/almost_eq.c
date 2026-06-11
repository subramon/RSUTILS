#include <stdbool.h>
#include <math.h>
#include <float.h>
bool 
almost_eq_relative_F4(
    float A, 
    float B, 
    float maxRelDiff
    )
{
  if ( maxRelDiff == 0 ) { maxRelDiff = FLT_EPSILON; }
    // Calculate the difference.
    float diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    float largest = (B > A) ? B : A;

    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}
bool 
almost_eq_relative_F8(
    double A, 
    double B, 
    double maxRelDiff
    )
{
  if ( maxRelDiff == 0 ) { maxRelDiff = DBL_EPSILON; }
    // Calculate the difference.
    double diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    double largest = (B > A) ? B : A;

    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}
