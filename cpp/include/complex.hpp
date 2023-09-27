/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Tomer Savorai  				    *
* Date: 20.06.2023						    	*
* Description: Header file for Complex      	*
* Version: 2.1                                  *
* Status: Aprroved                              *
*************************************************/
#ifndef MY_PROJECT_COMPLEX_HPP
#define MY_PROJECT_COMPLEX_HPP

#include <iosfwd> // ostream, istream

namespace dev
{

class Complex
{
public:
    inline Complex(const double real = 0, const double imaginary = 0);
    //CCtor, operator= and Dtor are auto-generated

    inline void SetReal(double num);
    inline void SetImaginary(double num);
    inline double GetReal() const;
    inline double GetImaginary() const;
    
    inline Complex& operator+=(const Complex& other_);
    inline Complex& operator-=(const Complex& other_);
    inline Complex& operator*=(const Complex& other_);
    inline Complex& operator/=(const Complex& other_);

private:
    double m_real;
    double m_imaginary;
};

/************************************ Foward Declarations ****************************************************/
inline const Complex operator+(const Complex& lhs_, const Complex& rhs_);
inline const Complex operator-(const Complex &lhs_, const Complex &rhs_);
inline const Complex operator*(const Complex &lhs_, const Complex &rhs_);
inline const Complex operator/(const Complex &lhs_, const Complex &rhs_);
bool operator==(const Complex &lhs_, const Complex &rhs_);
bool operator!=(const Complex& lhs_, const Complex& rhs_);
std::ostream& operator<<(std::ostream& os, const Complex& complex_);
std::istream& operator>>(std::istream& is, Complex& complex_);

/************************************ Inline Functions Definitions *******************************************/
inline Complex::Complex(const double real, const double imaginary) : 
                        m_real(real), m_imaginary(imaginary) {}

inline void Complex::SetReal(double num)
{
    m_real = num;
}

inline void Complex::SetImaginary(double num)
{
    m_imaginary = num;
}

inline double Complex::GetReal() const
{
    return m_real;
}

inline double Complex::GetImaginary() const
{
    return m_imaginary;
}

inline const Complex operator+(const Complex& lhs_, const Complex& rhs_)
{
    Complex value((lhs_.GetReal() + rhs_.GetReal()), 
                  (lhs_.GetImaginary() + rhs_.GetImaginary()));

    return value;
}

inline const Complex operator-(const Complex& lhs_, const Complex &rhs_)
{
    Complex value((lhs_.GetReal() - rhs_.GetReal()), 
                  (lhs_.GetImaginary() - rhs_.GetImaginary()));

    return value;
}

inline const Complex operator*(const Complex& lhs_, const Complex &rhs_)
{
    Complex value((lhs_.GetReal() * rhs_.GetReal()), 
                  (lhs_.GetImaginary() * rhs_.GetImaginary()));

    return value;
}

inline const Complex operator/(const Complex& lhs_, const Complex &rhs_)
{
    Complex value((lhs_.GetReal() / rhs_.GetReal()), 
                  (lhs_.GetImaginary() / rhs_.GetImaginary()));

    return value;
}

inline Complex& Complex::operator+=(const Complex& other_)
{
    *this = *this + other_;

    return *this;
}

inline Complex& Complex::operator-=(const Complex& other_)
{
    *this = *this - other_;

    return *this;
}

inline Complex& Complex::operator*=(const Complex& other_)
{
    *this = *this * other_;

    return *this;
}

inline Complex& Complex::operator/=(const Complex& other_)
{
    *this = *this / other_;

    return *this;
}
}

#endif // MY_PROJECT_COMPLEX_HPP