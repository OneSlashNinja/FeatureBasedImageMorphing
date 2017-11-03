#include "geometricTool.h"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------------------
//-------------------------Vector implements------------------------------------------------
//------------------------------------------------------------------------------------------
Vector2d::Vector2d(double vx, double vy){
  set(vx, vy);
}

Vector2d::Vector2d(const Vector2d &v){
  set(v);
}

//
// Array index form of a vector
// Routines returning an lvalue: i.e. X[i] returns addr of X[i]
//
double& Vector2d::operator[](int i)
{
  if(i < 0 || i > 1){
    cerr << "2D vector index bounds error" << endl;
    exit(1);
  }

  switch(i){
  case 0:
    return x;
  default:
    return y;
  }
}

//
// Array index form of a vector
// Routines returning an rvalue: i.e. X[i] returns contents of X[i]
//
const double& Vector2d::operator[](int i) const
{
  if(i < 0 || i > 1){
    cerr << "2D vector index bounds error" << endl;
    exit(1);
  }

  switch(i){
  case 0:
    return x;
  default:
    return y;
  }
}


// Compute the norm of a vector.
Vector2d Vector2d::normalize() const
{
  double magnitude;
  Vector2d newv;
  magnitude = norm();
  if (Abs(x) > magnitude * HUGENUMBER ||
      Abs(y) > magnitude * HUGENUMBER ){
    cerr << "Attempting to take the norm of a zero 2D vector." << endl;
  }
  newv.x = x / magnitude;
  newv.y = y / magnitude;
  return newv;
}


//  Set the components of a Vector according to the arguments
void Vector2d::set(double vx, double vy)
{
  x = vx;
  y = vy;
}
void Vector2d::set(const Vector2d &v)
{
  x = v.x;
  y = v.y;
}

// Print a Vector to the standard output device.
void Vector2d::print() const
{
  cout << "[" << x << " " << y << "]";
}
void Vector2d::print(int w, int p) const
{
  cout << "[" << setw(w) << setprecision(p) << Round(x, p) << " ";
  cout << setw(w) << setprecision(p) << Round(y, p) << "]";
}


// Compute the magnitude of a vector.
double Vector2d::norm() const
{
  return sqrt(normsqr());
}

// Compute the squared magnitude of a vector.
double Vector2d::normsqr() const
{
  return Sqr(x) + Sqr(y);
}

// Unary negation of a vector
Vector2d operator-(const Vector2d& v1){
  Vector2d r(-v1.x, -v1.y);
  return(r);
}

//  Addition of 2 Vectors.
Vector2d operator+(const Vector2d& v1, const Vector2d& v2)
{
  Vector2d r;
  r.x = v1.x + v2.x;
  r.y = v1.y + v2.y;
  return r;
}


//  Subtract two Vectors.
Vector2d operator-(const Vector2d& v1, const Vector2d& v2)
{
  Vector2d r;
  r.x = v1.x - v2.x;
  r.y = v1.y - v2.y;
  return r;
}

// Product of vector and scalar
Vector2d operator*(const Vector2d& v, double s)
{
  Vector2d r;

  r.x = v.x * s;
  r.y = v.y * s;
  return r;
}
Vector2d operator*(double s, const Vector2d& v)
{
  Vector2d r;

  r.x = v.x * s;
  r.y = v.y * s;
  return r;
}

// Inner product of two Vectors
double operator*(const Vector2d& v1, const Vector2d& v2)
{
  return(v1.x * v2.x +
	 v1.y * v2.y);
}

//  Component-wise multiplication of two Vectors
Vector2d operator^(const Vector2d& v1, const Vector2d& v2)
{
  Vector2d r;
  r.x = v1.x * v2.x;
  r.y = v1.y * v2.y;
  return r;
}


// Divide a vector by a scalar.
Vector2d operator/(const Vector2d& v, double s)
{
  Vector2d r;
  r.x = v.x / s;
  r.y = v.y / s;
  return(r);
}

// Determine if two Vectors are identical.
short operator==(const Vector2d& one, const Vector2d& two)
{
  return((one.x == two.x) && (one.y == two.y));
}

//------------------------------------------------------------------------------------------
//-------------------------Point implements-------------------------------------------
//------------------------------------------------------------------------------------------

//
// Helper function to draw a filled circle with center at position (x, y) and radius r
//
void Disk(double x, double y, double r)
{
  static const double CIRC_INC = 2 * PI / 16;
  double theta;

  glBegin(GL_POLYGON);
  for(theta = 0.0; theta < 2 * PI; theta += CIRC_INC)
    glVertex2f(x + r * cos(theta), y + r * sin(theta));
  glEnd();
}

Point::Point(){
}

Point::Point(const Vector2d &v){
  x = v.x;
  y = v.y;
}

int Point::hit(const Vector2d &cursor) const{
  return(Sqr(x - cursor.x) + Sqr(y - cursor.y) < Sqr(HIT_RADIUS));
}

void Point::draw() const{
  glColor3f(0.0, 0.0, 0.9);	// light red
  Disk(x, y, HIT_RADIUS);
}

//------------------------------------------------------------------------------------------
//-------------------------FeatureLine implements-------------------------------------------
//------------------------------------------------------------------------------------------
FeatureLine::FeatureLine() {
}

FeatureLine::FeatureLine(const Point& start, const Point& end) {
  startPoint = start;
  endPoint = end;
}

Vector2d FeatureLine::coordinate() {
  return endPoint - startPoint;
}

Point* FeatureLine::hitvertex(const Vector2d &cursor) {
  if ( startPoint.hit(cursor) ) return &startPoint;
  else if ( endPoint.hit(cursor) ) return &endPoint;
  return NULL;
}

void FeatureLine::draw() {

  startPoint.draw();
  endPoint.draw();
  
  glLineWidth(2);
  glColor3f(0.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex2f(startPoint.x, startPoint.y);
    glVertex2f(endPoint.x, endPoint.y);
  glEnd();
}

void FeatureLine::print() {
  cout << "( ";
  startPoint.print();
  cout << ", ";
  endPoint.print();
  cout << " ) " << endl;
}

/* Operators */
FeatureLine& FeatureLine::operator=(const FeatureLine& line) {
  startPoint = line.startPoint;
  endPoint = line.endPoint;
  return *this;
}

bool FeatureLine::operator==(const FeatureLine& line) {
  return ( (startPoint == line.startPoint) && (endPoint == line.endPoint) );
}

