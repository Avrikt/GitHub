/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Egor Markov      			    *
* Date: 04.07.2023						    	*
* Description: Source file for Shapes         	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <iostream> //cout
#include <iomanip>  //setw()
#include <vector>   //vector

class IShape
{
public:
    virtual void Draw() const = 0;
    virtual void Move(int distance) = 0;
    virtual ~IShape() {};
};

/**************Shape***************/
class Shape: public IShape
{
public:
    Shape(int offset_ = 0);
    virtual ~Shape() {};
    
protected:
    int GetOffset() const;
    void SetOffset(int offset_);

private:
    int m_offset;
};

Shape::Shape(int offset_) : m_offset(offset_) {}

int Shape::GetOffset() const
{
    return m_offset;
}

void Shape::SetOffset(int offset_)
{
    m_offset = offset_;
}

/**************Square***************/
class Square: public Shape
{
public:
    void Draw() const;
    void Move(int distance = 0);
};

void Square::Draw() const
{
    std::cout << std::setw(GetOffset()) <<  "I'm a Square." << std::endl;
}

void Square::Move(int distance)
{
    SetOffset(GetOffset() + distance);
}
/**************Circle***************/
class Circle: public Shape
{
public:
    void Draw() const;
    void Move(int distance = 0);
};

void Circle::Draw() const
{
    std::cout << std::setw(GetOffset()) <<  "I'm a Circle."<< std::endl;
}

void Circle::Move(int distance)
{
    SetOffset(GetOffset() + distance);
}

/**************Line***************/
class Line: public IShape
{
public:
    Line(int offset_ = 0);
    virtual ~Line() {};
    
    int GetOffset() const;
    void SetOffset(int offset_);

    void Draw() const;
    void Move(int distance);

private:
    int m_offset;
};

Line::Line(int offset_): m_offset(offset_) {}

int Line::GetOffset() const
{
    return m_offset;
}

void Line::SetOffset(int offset_)
{
    m_offset = offset_;
}

void Line::Draw() const
{
    std::cout << std::setw(GetOffset()) << "I'm a line."  << std::endl;
}

void Line::Move(int distance)
{
    SetOffset(GetOffset() + distance);
}

class Rectangle: public IShape
{
public:
    Rectangle(int offset_ = 0);
    virtual ~Rectangle() {};
    
    int GetOffset() const;
    void SetOffset(int offset_);

    void Draw() const;
    void Move(int distance);

private:
    int m_offset;
};

Rectangle::Rectangle(int offset_): m_offset(offset_) {}

int Rectangle::GetOffset() const
{
    return m_offset;
}

void Rectangle::SetOffset(int offset_)
{
    m_offset = offset_;
}

void Rectangle::Draw() const
{
    std::cout << std::setw(GetOffset()) << "I'm a Rectangle." << std::endl;
}

void Rectangle::Move(int distance)
{
    SetOffset(GetOffset() + distance);
}

int main(void)
{
    Square sq;
    Line ln;
    Circle cr;
    Rectangle rc;

    std::cout<< "\n\t\033[4;36m Square \033[0m" << std::endl;
    sq.Move(10);
    sq.Draw();

    std::cout<< "\n\t\033[4;36m Line \033[0m" << std::endl;
    ln.Move(10);
    ln.Draw();

    std::cout<< "\n\t\033[4;36m Circle \033[0m" << std::endl;
    cr.Move(10);
    cr.Draw();
    
    std::cout<< "\n\t\033[4;36m Rectangle \033[0m" << std::endl;
    rc.Move(10);
    rc.Draw();

    
    std::cout<< "\n\t\033[4;36m List of Shapes \033[0m" << std::endl;
    std::vector<IShape*> shapes_vec;

    shapes_vec.push_back(&sq);
    shapes_vec.push_back(&ln);
    shapes_vec.push_back(&cr);
    shapes_vec.push_back(&rc);

    for (size_t i = 0; i < shapes_vec.size(); ++i)
    {
        shapes_vec[i]->Move(i * 10);
        shapes_vec[i]->Draw();
    }
    
    return 0;
}