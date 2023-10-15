//Reuben Russell 23004666

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//------------------- Error Codes -------------------

// 0 - Program executed successfully.
// 2 - Failed to read from file
// 3 - Failed to write to file


//-------------- Classes and Functions --------------

class pixel_class {
private:
    uint8_t red = 0, green = 0, blue = 0;
    bool exclude = false, holeExclude = false;
public:
    uint8_t getR() { return red; }
    uint8_t getG() { return green; }
    uint8_t getB() { return blue; }
    void loaddata(int v1, int v2, int v3);
    void datatofile(fstream& ppmfile);
    void setexclude(bool ex) { exclude = ex; }
    bool getexclude() { return exclude; }
    void setholeexclude(bool ex) { holeExclude = ex; }
    bool getholeexclude() { return holeExclude; }
};

void loadButtons();
void findButton();
void drawBox(int x, int y);
void lookAround(int x, int y);
void findHole();
void lookIn(int x, int y);
void drawImage();

pixel_class picture[600][600];
int total, holes;
int xmin, xmax, ymin, ymax;
int screenx, screeny, maxcolours;


//----------------------- Main ----------------------

int main() {

    // Read in the image from Buttons.ppm
    loadButtons();
    
    // Identify buttons and draw boxes
    findButton();
    
    // Output the final .ppm file
    drawImage();

    return 0;
}


//--------------- Load Scan and Draw ----------------

// Load picture from file
void loadButtons() {
    // load the picture from Buttons.ppm
    int x, y, R, G, B;
    fstream infile;
    string infilename, line;
    infilename = "buttons.ppm";
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

// Scans picture for buttons
void findButton()
{
    for (int y = 0; y < screeny; y+=10)
    {
        for (int x = 0; x < screenx; x+=10)
        {
            if (picture[x][y].getB() > 128 && !picture[x][y].getexclude()){
                drawBox(x, y);
            }
        }
    }
}

// Draws appropriate box around a given button
void drawBox(int x, int y)
{
    total = 0;
    holes = 0;
    xmin = screenx;
    xmax = 0;
    ymin = screeny;
    ymax = 0;

    lookAround(x, y);

    if (total < 200) return;

    findHole();

    int color[3];

    // Check if its broken
    if (total > 1900 && total < 2500 && holes == 8)
    {
        // Green
        color[0] = 0x00;
        color[1] = 0xFF;
        color[2] = 0x00;
    }
    else
    {
        // Red
        color[0] = 0xFF;
        color[1] = 0x00;
        color[2] = 0x00;
    }


    // Draw box
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

// Looks around a button to find it all
void lookAround(int x, int y)
{
    if (picture[x][y].getexclude() == false && picture[x][y].getB() > 128 && x >= 1 && x < screenx && y >= 1 && y < screeny)
    {
        picture[x][y].setexclude(true);
        total++;

        if (x < xmin) xmin = x;
        else if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        else if (y > ymax) ymax = y;

        lookAround(x + 2, y);
        lookAround(x - 2, y);
        lookAround(x, y + 2);
        lookAround(x, y - 2);
    }
    else picture[x][y].setexclude(true);
}

// Scans button for holes
void findHole()
{
    for (int y = ymin; y < ymax; y += 1)
    {
        for (int x = xmin; x < xmax; x += 1)
        {
            if (picture[x][y].getB() <= 128 && !picture[x][y].getholeexclude()) {
                lookIn(x, y);
                holes++;
            }
        }
    }
}

// Looks around a hole to find it all
void lookIn(int x, int y)
{
    if (picture[x][y].getholeexclude() == false && picture[x][y].getB() <= 128 && x >= xmin && x < xmax && y >= ymin && y < ymax)
    {
        picture[x][y].setholeexclude(true);

        lookIn(x + 1, y);
        lookIn(x - 1, y);
        lookIn(x, y + 1);
        lookIn(x, y - 1);
    }
    else picture[x][y].setholeexclude(true);
}

// Load file from picture
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
    for (int y = 0; y < screeny; y++)
    {
        for (int x = 0; x < screenx; x++)
        {
            picture[x][y].datatofile(outfile);
        }
        outfile << endl;
    }
    outfile.close();
    cout << "Image saved to " << outfilename;
}


//------------- Methods for pixel_class -------------

// Load colors into pixel
void pixel_class::loaddata(int v1, int v2, int v3) {
    red    = v1;
    green  = v2;
    blue   = v3;
}

// Loads colors from pixel
void pixel_class::datatofile(fstream& ppmfile) {
    ppmfile <<  to_string(red)    << " "
            <<  to_string(green)  << " "
            <<  to_string(blue)   << "  ";
}
