//Reuben Russell 23004666

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

#include <ctime>

class pixel_class {
private:
    int red, green, blue;
    bool exclude;  // if true, do not check this pixel
public:
    void loaddata(int v1, int v2, int v3);
    void datatofile(fstream& ppmfile);
    int getR() { return red; }
    int getG() { return green; }
    int getB() { return blue; }
    void setexclude(bool ex) { exclude = ex; }
    bool getexclude() { return exclude; }
};

void loadButtons();
void drawImage();
void findButton();
void drawBox(int x, int y);
void lookAround(int x, int y);

uint32_t total, xmin, xmax, ymin, ymax;  // MUST be global if used
int screenx, screeny, maxcolours;   // you must use these
pixel_class picture[600][600];      // you must use this

int main() {

    // Step 1 : read in the image from Buttons.ppm
    loadButtons();
    
    // Step 2 : identify buttons and draw boxes
    findButton();
    
    // Step 3 : output the final .ppm file
    drawImage();

    return 0;
}

void loadButtons() {
    // load the picture from Buttons.ppm
    int x, y, R, G, B;
    fstream infile;
    string infilename, line;
    infilename = "Buttons.ppm";
    infile.open(infilename.c_str(), fstream::in);
    if (infile.is_open() == false) {
        cout << "ERROR: not able to open " << infilename << endl;
        exit(2);
    }
    getline(infile, line);  // this line is "P3"
    getline(infile, line);  // this line is "# filename"
    infile >> screenx >> screeny;  // this line is the size
    infile >> maxcolours;  // this line is 256
    for (y = 0; y < screeny; y++) {
        for (x = 0; x < screenx; x++) {
            infile >> R >> G >> B;
            picture[x][y].loaddata(R, G, B);
            picture[x][y].setexclude(false);
        }
    }
    infile.close();
}

void drawImage()
{
    // load the picture to output.ppm
    fstream outfile;
    string outfilename;
    outfilename = "Output.ppm";
    outfile.open(outfilename.c_str(), fstream::out);
    if (outfile.is_open() == false) {
        cout << "ERROR: not able to open " << outfilename << endl;
        exit(3);
    }
    outfile << "P3\n";  // this line is "P3"
    outfile << "# " + outfilename + '\n';  // this line is "# filename"
    outfile << to_string(screenx) + ' ' << to_string(screeny) + '\n';  // this line is the size
    outfile << to_string(maxcolours) + '\n';  // this line is 256
    for (int y = 0; y < screeny; y++) {
        for (int x = 0; x < screenx; x++) {
            outfile << to_string(picture[x][y].getR()) + " "
                    << to_string(picture[x][y].getG()) + " "
                    << to_string(picture[x][y].getB()) + " ";
        }
        outfile << endl;
    }
    outfile.close();
}

void findButton()
{
    for (int y = 0; y < screeny; y++)
    {
        for (int x = 0; x < screenx; x++)
        {
            if (picture[x][y].getB() > 128 && !picture[x][y].getexclude()){
                drawBox(x, y);
            }
        }
    }
}

void drawBox(int x, int y)
{
    total = 0;
    xmin = screenx;
    xmax = 0;
    ymin = screeny;
    ymax = 0;

    lookAround(x, y);

    int color[3];
    if (total > 7700)
    {
        color[0] = 0;
        color[1] = 255;
        color[2] = 0;
    }
    else
    {
        color[0] = 255;
        color[1] = 0;
        color[2] = 0;
    }

    for (int i = xmin; i < xmax; i++)
    {
        picture[i][ymax].loaddata(color[0], color[1], color[2]);
        picture[i][ymin].loaddata(color[0], color[1], color[2]);
    }
    for (int j = ymin; j < ymax; j++)
    {
        picture[xmin][j].loaddata(color[0], color[1], color[2]);
        picture[xmax][j].loaddata(color[0], color[1], color[2]);
    }
}

void lookAround(int x, int y)
{
    if (picture[x][y].getexclude() == false && picture[x][y].getB() > 128)
    {
        picture[x][y].setexclude(true);
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
        total++;

        lookAround(x + 1, y);
        lookAround(x - 1, y);
        lookAround(x, y + 1);
        lookAround(x, y - 1);
    }
    else 
    {
        picture[x][y].setexclude(true);
        return;
    }
}

//--------------- methods for the pixel_class ------------
void pixel_class::loaddata(int v1, int v2, int v3) {
    red = v1;
    green = v2;
    blue = v3;
}

void pixel_class::datatofile(fstream& ppmfile) {
    // write the data for one pixel to the ppm file
    ppmfile << red << " " << green;
    ppmfile << " " << blue << "  ";
}
