#pragma once

class Complex {
public:
    Complex(void) = default;
    Complex(double Re, double Im = 0.0);
    
    Complex operator + (void);
    Complex operator - (void);
    
    Complex& operator += (const Complex& z);
    Complex& operator -= (const Complex& z);
    Complex& operator *= (const Complex& z);

    double GetRe(void) const;
    double GetIm(void) const;
    
private:
    double Re_ = 0.0;
    double Im_ = 0.0;
};

Complex operator + (const Complex& z1, const Complex& z2);
Complex operator - (const Complex& z1, const Complex& z2);
Complex operator * (const Complex& z1, const Complex& z2);

bool operator == (const Complex& z1, const Complex& z2);
bool operator != (const Complex& z1, const Complex& z2);

double Abs(const Complex& z);
