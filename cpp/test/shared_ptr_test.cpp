/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Adir Ortal   				    *
* Date: 03.08.2023						    	*
* Description: Test file for Shared Pointer  	*
* Version: 1.1                                  *
* Status: Approved                              *
*************************************************/
#include <iostream>

#include "shared_ptr.hpp"
#include "utilities.hpp"

using namespace dev;

class B
{
public:
    B(int data = 0);
    virtual ~B(){}
    virtual float GetData() const {return m_data;}

private:
    int m_data;
};

B::B(int data) : m_data(data) {}

class D: public B
{
public:
    D(int val_ = 0, float f = 0): B(val_), f(f) {}

    float GetData() const {return f;}

private:
    float f;
};

int main(void)
{  
    HEADER(Constructors);
    SharedPtr<int> sh_i(new int(10));
    SharedPtr<int> sh_i2(sh_i);
    TEST(*sh_i == 10, Ctor);
    TEST(*sh_i2 == 10, CCtor);

    HEADER(Operator->);
    SharedPtr<B> sh_b(new B(20));
    SharedPtr<D> sh_d(new D(40,30.0));
    TEST(20 == sh_b->GetData(), operator->);
    TEST(30.0 == sh_d->GetData(), operator->);

    HEADER(Operator=);
    SharedPtr<B> sh_b3(new B);
    sh_b3 = sh_b;
    TEST(20 == sh_b3->GetData(), operator=);
    
    SharedPtr<B> sh_b2(sh_d);
    TEST(30 == sh_b2->GetData(), operator=);

    HEADER(Polymorphism);
    SharedPtr<B> sh_b_d = new D(15, 3.1);
    TEST(3.1f == float(sh_b_d->GetData()), Polymorphism);
    return 0;
}