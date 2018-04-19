// commands to run the program:
// g++ main.cpp -lglut -lGL -lGLU -lGLEW -o main
// ./main

#include <iostream>
#include <string>
#include <math.h>
#include "GL/freeglut.h"
#include "GL/gl.h"

int shape;
int algorithm;
double lineX1;
double lineY1;
double lineX2;
double lineY2;
int xc;
int yc;
int r;
int r2;

// start drawDDALine

float round_value(float v)
{
    return floor(v + 0.5);
}

void drawDDALine(double x1, double y1, double x2, double y2)
{
    glClear(GL_COLOR_BUFFER_BIT);
    double dx = (x2 - x1);
    double dy = (y2 - y1);
    double steps;
    float xInc, yInc, x = x1, y = y1;
    /* Find out whether to increment x or y */
    steps = (abs(dx) > abs(dy)) ? (abs(dx)) : (abs(dy));
    xInc = dx / (float)steps;
    yInc = dy / (float)steps;

    /* Clears buffers to preset values */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Plot the points */
    glBegin(GL_POINTS);
    /* Plot the first point */
    glVertex2d(x, y);
    int k;
    /* For every step, find an intermediate vertex */
    for (k = 0; k < steps; k++)
    {
        x += xInc;
        y += yInc;
        /* printf("%0.6lf %0.6lf\n",floor(x), floor(y)); */
        glVertex2d(round_value(x), round_value(y));
    }
}
// end drawDDALine

// start drawBresenhamLine

void draw_pixel(double x, double y)
{

    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawBresenhamLine(double x1, double y1, double x2, double y2)
{
    glClear(GL_COLOR_BUFFER_BIT);
    double dx, dy, i, e;
    double incx, incy, inc1, inc2;
    double x, y;

    dx = x2 - x1;
    dy = y2 - y1;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    incx = 1;
    if (x2 < x1)
        incx = -1;
    incy = 1;
    if (y2 < y1)
        incy = -1;
    x = x1;
    y = y1;
    if (dx > dy)
    {
        draw_pixel(x, y);
        e = 2 * dy - dx;
        inc1 = 2 * (dy - dx);
        inc2 = 2 * dy;
        for (i = 0; i < dx; i++)
        {
            if (e >= 0)
            {
                y += incy;
                e += inc1;
            }
            else
                e += inc2;
            x += incx;
            draw_pixel(x, y);
        }
    }
    else
    {
        draw_pixel(x, y);
        e = 2 * dx - dy;
        inc1 = 2 * (dx - dy);
        inc2 = 2 * dx;
        for (i = 0; i < dy; i++)
        {
            if (e >= 0)
            {
                x += incx;
                e += inc1;
            }
            else
                e += inc2;
            y += incy;
            draw_pixel(x, y);
        }
    }
}

// end drawBresenhamLine

// start Function to draw a circle using bresenham's

void plot_point(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc - y, yc - x);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc - y, yc + x);
    glEnd();
}

void drawBresenhamCircle(int r)
{
    glClear(GL_COLOR_BUFFER_BIT);
    int x = 0, y = r;
    float pk = (5.0 / 4.0) - r;

    /* Plot the points */
    /* Plot the first point */
    plot_point(x, y);
    int k;
    /* Find all vertices till x=y */
    while (x < y)
    {
        x = x + 1;
        if (pk < 0)
            pk = pk + 2 * x + 1;
        else
        {
            y = y - 1;
            pk = pk + 2 * (x - y) + 1;
        }
        plot_point(x, y);
    }
}

// end Function to draw a circle using bresenham's

// start MidPoint Circle

void plot(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x + xc, y + yc);
    glEnd();
}

void drawMidPointCircle()
{
    glClear(GL_COLOR_BUFFER_BIT);
    int x = 0;
    int y = r;
    float decision = 5 / 4 - r;
    plot(x, y);

    while (y > x)
    {
        if (decision < 0)
        {
            x++;
            decision += 2 * x + 1;
        }
        else
        {
            y--;
            x++;
            decision += 2 * (x - y) + 1;
        }
        plot(x, y);
        plot(x, -y);
        plot(-x, y);
        plot(-x, -y);
        plot(y, x);
        plot(-y, x);
        plot(y, -x);
        plot(-y, -x);
    }
}

// end MidPoint Circle

// start Ellipse
void SetPixel(GLint x, GLint y)
{
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void drawEllipse(int xCenter, int yCenter, int rx, int ry)
{
    glClear(GL_COLOR_BUFFER_BIT);
    float p;
    //Region 1
    p = ry * ry - rx * rx * ry + rx * rx / 4;
    int x = 0;
    int y = ry;
    while (2.0 * ry * ry * x <= 2.0 * rx * rx * y)
    {
        if (p < 0)
        {
            x++;
            p = p + 2 * ry * ry * x + ry * ry;
        }
        else
        {
            x++;
            y--;
            p = p + 2 * ry * ry * x - 2 * rx * rx * y - ry * ry;
        }
        SetPixel(xc + x, yc + y);
        SetPixel(xc + x, yc - y);
        SetPixel(xc - x, yc + y);
        SetPixel(xc - x, yc - y);
    }

    //Region 2
    p = ry * ry * (x + 0.5) * (x + 0.5) + rx * rx * (y - 1) * (y - 1) - rx * rx * ry * ry;
    while (y > 0)
    {
        if (p <= 0)
        {
            x++;
            y--;
            p = p + 2 * ry * ry * x - 2 * rx * rx * y + rx * rx;
        }
        else
        {
            y--;
            p = p - 2 * rx * rx * y + rx * rx;
        }
        SetPixel(xc + x, yc + y);
        SetPixel(xc + x, yc - y);
        SetPixel(xc - x, yc + y);
        SetPixel(xc - x, yc - y);
    }
}

void myDisplay()
{
    if (shape == 1)
    {
        if (algorithm == 1)
        {
            drawBresenhamLine(lineX1, lineY1, lineX2, lineY2);
        }
        else
        {
            drawDDALine(lineX1, lineY1, lineX2, lineY2);
        }
    }
    else if (shape == 2)
    {
        if (algorithm == 1)
        {
            drawBresenhamCircle(r);
        }
        else
        {
            drawMidPointCircle();
        }
    }
    else if (shape == 3)
    {
        drawEllipse(xc, yc, r, r2);
    }

    glEnd();
    glFlush();
}

void Init()
{
    /* Set clear color to white */
    glClearColor(1.0, 1.0, 1.0, 0);
    /* Set fill color to black */
    glColor3f(0.0, 0.0, 0.0);
    gluOrtho2D(0, 640, 0, 480);
}

int main(int argc, char **argv)
{

    std::cout << "Please enter shape (Line - 1, Circle - 2, Ellipse - 3) :\n";
    std::cin >> shape;

    if (shape == 1)
    {
        std::cout << "Shape will be: Line \n";
        std::cout << "Please choose algorithm (Brensenham - 1, DDA - 2) :\n";
        std::cin >> algorithm;
        if (algorithm == 1)
        {
            std::cout << "Algorithm will be: Brensenham \n";
        }
        else
        {
            std::cout << "Algorithm will be: DDA \n";
        }
        std::cout << "Please enter start X coordinate :\n";
        std::cin >> lineX1;
        std::cout << "Please enter start Y coordinate :\n";
        std::cin >> lineY1;
        std::cout << "Please enter end X coordinate :\n";
        std::cin >> lineX2;
        std::cout << "Please enter end Y coordinate :\n";
        std::cin >> lineY2;
    }
    else if (shape == 2)
    {
        std::cout << "Shape will be: Circle \n";
        std::cout << "Please choose algorithm (Brensenham - 1, MidPoint - 2) :\n";
        std::cin >> algorithm;
        if (algorithm == 1)
        {
            std::cout << "Algorithm will be: Brensenham \n";
        }
        else
        {
            std::cout << "Algorithm will be: MidPoint \n";
        }
        std::cout << "Please enter center X coordinate :\n";
        std::cin >> xc;
        std::cout << "Please enter center Y coordinate :\n";
        std::cin >> yc;
        std::cout << "Please enter radius of circle :\n";
        std::cin >> r;
    }
    else if (shape == 3)
    {
        std::cout << "Shape will be: Ellipse \n";
        std::cout << "Please enter center X coordinate :\n";
        std::cin >> xc;
        std::cout << "Please enter center Y coordinate :\n";
        std::cin >> yc;
        std::cout << "Please enter radius 1 of ellipse :\n";
        std::cin >> r;
        std::cout << "Please enter radius 2 of ellipse :\n";
        std::cin >> r2;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Drawings");
    Init();
    glutDisplayFunc(myDisplay);
    glutMainLoop();
    return 0;
}