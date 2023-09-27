/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Tomer Savorai  				    *
* Date: 20.06.2023						    	*
* Description: Source file for Complex      	*
* Version: 2.1                                  *
* Status: Aprroved                              *
*************************************************/

#include "complex.hpp"
#include "utilities.hpp"

namespace dev
{
bool operator==(const Complex& lhs_, const Complex& rhs_)
{
    return (lhs_.GetReal() == rhs_.GetReal()) && 
           (lhs_.GetImaginary() == rhs_.GetImaginary());
}

bool operator!=(const Complex& lhs_, const Complex& rhs_)
{
    return !(lhs_ == rhs_);
}

std::ostream& operator<<(std::ostream& os, const Complex& complex_)
{
    return os << complex_.GetReal() << " + " 
              << complex_.GetImaginary() << "i";
}

std::istream& operator>>(std::istream& is, Complex& complex_)
{
    double real(0);
    double imag(0);

    is >> real >> imag;

    complex_ = Complex(real, imag);

    return is;
}
}
