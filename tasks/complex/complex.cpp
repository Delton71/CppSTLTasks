#include "complex.hpp"
#include <cmath>


Complex::Complex(double Re, double Im): Re_(Re), Im_(Im) {}

Complex Complex::operator + (void) {
    Complex copy;
    copy.Re_ = Re_;
    copy.Im_ = Im_;

    return copy;
}

Complex Complex::operator - (void) {
    Complex copy;
    copy.Re_ = -Re_;
    copy.Im_ = -Im_;

    return copy;
}

Complex& Complex::operator += (const Complex& z) {
    Re_ += z.Re_;
    Im_ += z.Im_;
    return *this;
}

Complex& Complex::operator -= (const Complex& z) {
    Re_ -= z.Re_;
    Im_ -= z.Im_;
    return *this;
}

Complex& Complex::operator *= (const Complex& z) {
    double NewIm = Re_ * z.Im_ + Im_ * z.Re_;
    Re_ = Re_ * z.Re_ - Im_ * z.Im_;
    Im_ = NewIm;
    return *this;
}

double Complex::GetRe(void) const {
    return Re_;
}

double Complex::GetIm(void) const {
    return Im_;
}

Complex operator + (const Complex& z1, const Complex& z2) {
    Complex ZAns = z1;
    ZAns += z2;
    return ZAns;
}

Complex operator - (const Complex& z1, const Complex& z2) {
    Complex ZAns = z1;
    ZAns -= z2;
    return ZAns;
}

Complex operator * (const Complex& z1, const Complex& z2) {
    Complex ZAns = z1;
    ZAns *= z2;
    return ZAns;
}

bool operator == (const Complex& z1, const Complex& z2) {
    return (z1.GetRe() == z2.GetRe()) && (z1.GetIm() == z2.GetIm());
}

bool operator != (const Complex& z1, const Complex& z2) {
    return (z1.GetRe() != z2.GetRe()) || (z1.GetIm() != z2.GetIm());
}

double Abs(const Complex& z) {
    double Re = z.GetRe();
    double Im = z.GetIm();
    
    return sqrt(Re * Re + Im * Im);
}
