#include<iostream>
#include<string>
using namespace std;

class Shape{
   protected:
      string color;

   public:
      Shape(string c): color(c){}
      virtual double area() const = 0;   
      virtual void draw() const = 0;
      string getColor() {return color;}
      virtual ~Shape(){}
};

class Circle : public Shape{
    private:
      double radius;
    public:
      Circle(string c,double r) : Shape(c), radius(r){}
      
      double area() const override {
        return 3.14159 * radius * radius;
      }

      void draw() const override{
        cout << "Drawing a " << color << " circle (area: " << area() << ")" << endl;
      }
};

class Rectangle: public Shape{
    private:
      double length;
      double width;
    
    public:
      Rectangle(string c, double l, double w) : Shape(c), length(l), width(w) {}
      
      double area() const override{
        return length*width;
      }

      void draw() const override{
        cout << "Drawing a " << color << " rectangle (area: " << area() << ")" << endl;
      }
};

int main(){
    Shape* shapes[2];
    shapes[0]=new Circle("Red",5.0);
    shapes[1]=new Rectangle("Blue",4.0,6.0);

    for (int i = 0; i < 2; i++)
    {
        shapes[i]->draw();
    }

    for (int i = 0; i < 2; i++)
    {
        delete shapes[i];
    }
    return 0;   
}