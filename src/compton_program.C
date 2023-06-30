/* 
This is a program I'm writing for the Compton lab experience and it should do everything you would want to do:
    - calculating the expected cross section for the Klein-Nishina part of the experiment;
    - computing the solid angle needed for this same part;
    - fit of gaussian peaks with various backgrounds;
    - linear fit for the activity measurements;
    - ... (anything else that might be remotely useful and is going to be added in the future).

To compile, on the terminal: g++ compton_program.C `root-config --glibs --cflags` -o compton
and after compiling, ./compton name_of_file.txt "Label for the x-axis" "Label for the y-axis" "Title of the graph"
*/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include "TStyle.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"
#include "TAxis.h"
#include "TFitResult.h"
#include "TGraph.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "comptonLib.h"
#define _USE_MATH_DEFINES

// -------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------ main -------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------

int main (int argc, char ** argv)
{
    TApplication* myApp = new TApplication ("myApp", NULL, NULL);

    if (argc < 5)
    {
        std::cout << "\n Attention: for the program to start, you need to insert:\n\t\t-the name of a .txt file containing the data;\n\t\t-the name of the x axis, y axis and of the graph." << std::endl;
        return 1;
    }

    // -------------- Calculating the solid angle, if needed -----------------
    
    int choice = 0;
    double solid_ang = 0;
    double distance = 31.5;
    double radius = 5.08/2;

    std::cout << "\t\tInsert 0 if you want to calculate the solid angle, 1 if not." << std::endl;
    std::cin >> choice;
    switch (choice)
    {
        case 0: 
            solid_ang = solidAngle(distance, radius);
            std::cout << "Solid angle = " << solid_ang << std::endl;
            break;
        case 1:
            std::cout << " Not calculating the solid angle." << std::endl;
            break;
    }
    choice = 0;

    // ------------------------- Cross section computation, if needed ----------------------------------

    std::cout << "\t\tDo you need to compute the cross section? 0 for y, 1 for n.\n" << std::endl;
    std::cin >> choice;
    double angles[] = {10*M_PI/180, 20*M_PI/180, 30*M_PI/180, 40*M_PI/180, 50*M_PI/180, 60*M_PI/180, 70*M_PI/180, 80*M_PI/180, 90*M_PI/180, 110*M_PI/180, 130*M_PI/180};
    double cross_section = 0;

    switch (choice)
    {
        case 0:
            for (int i = 0; i < 10; i++)
            {
                cross_section = computeCrossSection(angles[i]);
                std::cout << "\tThe expected value of the cross section for theta = " << angles[i]*180/M_PI << " is = " << cross_section << "barn.\n\n";
                cross_section = 0;
            }
            break;
        case 1:
            std::cout << " Not calculating the cross section." << std::endl;
            break;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------------------
    // --------------- Starting a fit and reading measurements from a .txt file -------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------------------------------------------------

    const char *nome_input = argv[1]; // The first argument on the command line is the name of the .txt file
    const char *x_axis_title = argv[2];
    const char *y_axis_title = argv[3];
    const char *graph_title = argv[4];

    choice = 0;
    std::cout << "\t\tWhat do you need to do? Insert:\n\t\t\t- 0 for a gaussian fit with an erf-background;\n\t\t\t- 1 for a gaussian fit with an exponential background;\n\t\t\t- 2 for a linear fit;\n\t\t- 3 for calibration results; etc" << std::endl;
    std::cin >> choice;

    std::vector<double> params;

    switch (choice)
    {
        case 0:
            std::cout << "\n\n\n----------------- Gaussian fit with an erfc background -----------------\n\n\n" << std::endl;
            params = gaussFitErfcBack(nome_input, x_axis_title, y_axis_title, graph_title, 1200);
            break;
        case 1:
            std::cout << "\n\n\n----------------- Gaussian fit with an exponential background -----------------\n\n\n" << std::endl;
            params = gaussFitExpBack(nome_input, x_axis_title, y_axis_title, graph_title, 1200);
            break;
        case 2:
            std::cout << "\n\n\n----------------- Linear fit -----------------\n\n\n" << std::endl;
            params = linFit(nome_input, x_axis_title, y_axis_title, graph_title);
            break;
        case 3: 
            std::cout << "\n\n\n\n don't choose this option! i'm going to do this later\n\n\n" << std::endl;
            break;
    }

    /*

    TF1* gausFin = new TF1("gausFin", "gaus(0)");
    gausFin -> FixParameter (0, amp);
    gausFin -> FixParameter (1, centr);
    gausFin -> FixParameter (2, sigmaG);

    double integral = gausFin -> Integral( centr - 5*sigmaG, centr + 5*sigmaG);
    double integralErr = gausFin -> IntegralError( centr - 5*sigmaG, centr + 5*sigmaG);

    */

    myApp -> Run();
    return 0;
}


