#include <iostream>   // library for reading & writing from the console/keyboard
#include <cmath>      // library with the square root function & absolute value
#include <cstdlib>    // library with the exit function


// Returns true if the candidate root is indeed a root of the polynomial a*x*x + b*x + c = 0 
bool check_root(int a, int b, int c, float root) {  
  // plug the value into the formula
  float check = a * root * root + b * root + c;
  // see if the absolute value is zero (within a small tolerance)
  if (fabs(check) > 0.0001) {
    std::cerr << "ERROR:  " << root << " is not a root of this formula." << std::endl;
    return false;
  } else {
    return true;
  }
}

/* Use the quadratic formula to find the two real roots of polynomial.   Returns 
true if the roots are real, returns false if the roots are imaginary.  If the roots 
are real, they are returned through the reference parameters root_pos and root_neg. */ 
bool find_roots(int a, int b, int c, float &root_pos, float &root_neg) {
  // compute the quantity under the radical of the quadratic formula
  int radical = b*b - 4*a*c;
  // if the radical is negative, the roots are imaginary
  if (radical < 0) {
    
    return false;
  }
  float sqrt_radical = sqrt(radical);  
  // compute the two roots
  root_pos = (-b + sqrt_radical) / 2*a;
  root_neg = (-b - sqrt_radical) / 2*a;
  return true;
}

int main() {
  
  int b_values[5];
  int c_values[5];
  for (int i = 0; i < 5; i += 1) {
	  std::cout << "Enter 2 integer coefficients to a quadratic function: x*x + b*x + c = 0" << std::endl;
	  std::cin >> b_values[i] >> c_values[i];
  }
  
  for (int i = 0; i < 5; i += 1) {
	  float root_1, root_2;
	  bool success = find_roots(1,b_values[i],c_values[i], root_1,root_2);
	  if (!success) {
		  std::cerr << "ERROR:  Imaginary roots" << std::endl;
	  } else {
		  std::cout << "The roots are: " << root_1 << " and " << root_2 <<std::endl;
	  }
  }
  
  // by convention, main should return zero when the program finishes normally
  return 0; 
}