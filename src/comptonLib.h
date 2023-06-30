#ifndef comptonLib_h

#define comptonLib_h

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

// -------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------- Inizialization of functions needed in the program -----------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------

double solidAngle(double d, double a); // d is the distance from the scatterator to the detector, a is the radius of the detector
double computeCrossSection(double ang);

void plotHisto(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name);

std::vector<double> calibration(const char *file_name);
std::vector<double> gaussFitErfcBack(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name, int guess);
std::vector<double> gaussFitExpBack(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name, int guess);
std::vector<double> gaussFitLinBack(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name, int guess);
std::vector<double> linFit(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name);
/*
std::vector<double> comptonFit(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name);
std::vector<double> kleinNishinaFit(const char* file_name, const char *x_axis, const char *y_axis, const char *graph_name);
*/


// -------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------

double solidAngle(double d, double a)
{
	double solid_ang = 0;
	
	solid_ang = 2*M_PI*( 1 - cos( atan(a/d) ) );
	return solid_ang;
}

double computeCrossSection(double ang)
{
	double cross_sec = 0;
	
	cross_sec = (pow(1/137, 2)) * (1 / (2*pow(0.511, 2)) ) * ( pow(1/(2-cos(ang)), 2) ) * (2 - cos(ang) + (1/(2-cos(ang))) - pow(sin(ang), 2) );
	cross_sec = cross_sec * 200 * 200; // in fm^2
	cross_sec = cross_sec / 100; // in barn

	return cross_sec;
}

void plotHisto(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name)
{
	std::ifstream input_file; 
	input_file.open(file_name, std::ios::in);

	std::vector<double> measures; 
	double mis = 0; 
	int count = 0;

	while (true)
	{
		input_file >> mis; 
		count++;
		if (input_file.eof() == true) 
			break;
		measures.push_back(mis);
	}

	input_file.close();

	TCanvas* c = new TCanvas();
	int nBin = measures.size();
	TH1F* h1 = new TH1F("h1", file_name, nBin, 0, 2048 );

	for(int i = 0; i < measures.size(); i++)
		h1 -> SetBinContent(i+1, measures.at(i));

	h1 -> GetXaxis() -> SetTitle(x_axis);
	h1 -> GetYaxis() -> SetTitle(y_axis);
	h1 -> SetTitle(graph_name);

	h1 -> Draw();
	// Add any graphic options

	return;
}

// Still working on some of these functions.

/*
vector<double> calibration(const char *file_name)   // To calibrate: y = energy, x = bin: model is y = ax^2 + bx + c 
{
	// We took note of where the peaks were (number of ch. for the fit guesses) by opening the spectra in Maestro.

	vector<double> measured;

	const char *th_238 = "Th228.txt"; // observed peaks at ch. & &
	const char *am_241 = "Am241.txt"; // observed peaks at ch. & & 
	const char *co_57  = "Co57.txt";  // observed peaks at ch. & & 
	const char *na_22  = "Na22.txt";  // observed peaks at ch. & & 
	const char *ra_226 = "Ra226.txt"; // observed peaks at ch. & &

	const char *xaxis = "ADC";
	const char *yaxis = "Counts";
	const char *graphname = "Peaks for calibration";

	vector<double> measured;
	vector<double> error_measured;

	vector<double> th_238_params = gaussFitErfcBack(th_238, xaxis, yaxis, graphname, 300);
	measured.push_back( th_238_params.at(0) );
	error_measured.push_back( th_238_params.at(3) );

	vector<double> am_241_params = gaussFitErfcBack(am_241, xaxis, yaxis, graphname, 300);
	measured.push_back( am_241_params.at(0) );
	error_measured.push_back( am_241_params.at(3) );

	vector<double> co_57_params = gaussFitErfcBack(co_57, xaxis, yaxis, graphname, 300);
	measured.push_back( co_57_params.at(0) );
	error_measured.push_back( co_57_params.at(3) );

	vector<double> na_22_params = gaussFitErfcBack(na_22, xaxis, yaxis, graphname, 300);
	measured.push_back( na_22_params.at(0) );
	error_measured.push_back( na_22_params.at(3) );

	TCanvas *c2 = new TCanvas();
	c2 -> Divide(3,3); // divides the canvas into three rows and three columns
	c2 -> cd(1);
	th228_graph -> Draw();
	c2 -> cd(2);
	myGraph2->Draw("apl");
	c2 -> cd(3);

	vector<double> expected{ }; // expected values for the nominal energies
	vector<double> x_errors(10, 0);
	
	TGraphErrors * final_graph = new TGraphErrors(expected, measured, x_errors, error_measured);

    final_graph -> GetXaxis() -> SetTitle("Energy [KeV]");
    final_graph -> GetYaxis() -> SetTitle("");
    final_graph -> SetTitle(graph_name);
   
    graph -> Fit("pol2");
    graph -> Draw();
    gStyle -> SetOptFit(1111);

	TFitResultPtr r = graph -> Fit("pol2", "S");  // pol2 is ax2 + bx + c
	double a = r -> Parameter(0);
	double b = r -> Parameter(1);
	double c = r -> Parameter(2);

	std::vector<double> calibrated;
	calibrated.push_back(a);
	calibrated.push_back(b);
	calibrated.push_back(c);

	return calibrated;
}
*/

std::vector<double> gaussFitErfcBack(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name, int guess)
{
	std::ifstream input_file; 
	input_file.open(file_name, std::ios::in);

	std::vector<double> measures; 
	double mis = 0; 
	int count = 0;

	while (true)
	{
		input_file >> mis; 
		count++;
		if (input_file.eof() == true) 
			break;
		measures.push_back(mis);
	}

	input_file.close();

	TCanvas* c = new TCanvas();
	int nBin = measures.size();
	TH1F* h1 = new TH1F("h1", graph_name, nBin, 0, 2048);

	for(int i = 0; i < measures.size(); i++)
		h1 -> SetBinContent(i+1, measures.at(i));

	h1 -> GetXaxis() -> SetTitle(x_axis);
	h1 -> GetYaxis() -> SetTitle(y_axis);
	h1 -> SetTitle(graph_name);

	double p0  = 1200;      // Still need to actually understand what this parameter is for
	double p1 = guess;       // center of the gaussian function
	double p2  = 15 ;     
	double max = 0; 

	TF1* model = new TF1("model", "gaus(0) + [3]+[4]*TMath::Erfc((x-[1])/[2])");
	model -> SetParameter (0, p0); 
	model -> SetParameter (1, p1);
	model -> SetParameter (2, p2);
	model -> SetParameter (3, 50);
	max = p1;

	model -> SetRange (max - 300, max + 300);  // maybe useless
	model -> SetLineColor (kRed);

	h1 -> GetXaxis() -> SetRangeUser(max - 300, max + 300);
	h1 -> Fit("model", "R");

	gStyle -> SetOptFit(1111);
	h1 -> Draw();
	model -> Draw("same");

	TFitResultPtr r = h1 -> Fit("model", "S");  
	double amp = r -> Parameter(0);
	double centr = r -> Parameter(1);
	double sigmaG = r -> Parameter(2);
	double err_amp = r -> ParError(0);
	double err_mu = r -> ParError(1);
	double err_sigma = r -> ParError(2);

	std::vector<double> parameters;
	parameters.push_back(amp);
	parameters.push_back(centr);
	parameters.push_back(sigmaG);
	parameters.push_back(err_amp);
	parameters.push_back(err_mu);
	parameters.push_back(err_sigma);

	return parameters;
}

std::vector<double> gaussFitExpBack(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name, int guess)
{
	std::ifstream input_file; 
	input_file.open(file_name, std::ios::in);

	std::vector<double> measures; 
	double mis = 0; 
	int count = 0;

	while (true)
	{
		input_file >> mis; 
		count++;
		if (input_file.eof() == true) 
			break;
		measures.push_back(mis);
	}

	input_file.close();

	TCanvas* c = new TCanvas();
	int nBin = measures.size();
	TH1F* h1 = new TH1F("h1", graph_name, nBin, 0, 2048);

	for(int i = 0; i < measures.size(); i++)
		h1 -> SetBinContent(i+1, measures.at(i));

	h1 -> GetXaxis() -> SetTitle(x_axis);
	h1 -> GetYaxis() -> SetTitle(y_axis);
	h1 -> SetTitle(graph_name);

	double p0  = 1200; 
	double p1 = guess;    
	double p2  = 15 ;       
	double max = 0; 

	TF1* model = new TF1("model", "gaus(0) + TMath::Exp(-x/[3]))*[4])");
	model -> SetParameter (0, p0); 
	model -> SetParameter (1, p1);
	model -> SetParameter (2, p2);
	model -> SetParameter (3, 50);
	model -> SetParameter (4, 50);
	max = p1;

	model -> SetRange (max - 300, max + 300);  // maybe useless
	model -> SetLineColor (kRed);

	h1 -> GetXaxis() -> SetRangeUser(max - 300, max + 300);
	h1 -> Fit("model", "R");

	gStyle -> SetOptFit(1111);
	h1 -> Draw();
	model -> Draw("same");

	TFitResultPtr r = h1 -> Fit("model", "S");  
	double amp = r -> Parameter(0);
	double centr = r -> Parameter(1);
	double sigmaG = r -> Parameter(2);
	double err_amp = r -> ParError(0);
	double err_mu = r -> ParError(1);
	double err_sigma = r -> ParError(2);

	std::vector<double> parameters;
	parameters.push_back(amp);
	parameters.push_back(centr);
	parameters.push_back(sigmaG);
	parameters.push_back(err_amp);
	parameters.push_back(err_mu);
	parameters.push_back(err_sigma);

	return parameters;
}

std::vector<double> gaussFitLinBack(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name, int guess)
{
	std::ifstream input_file; 
	input_file.open(file_name, std::ios::in);

	std::vector<double> measures; 
	double mis = 0; 
	int count = 0;

	while (true)
	{
		input_file >> mis; 
		count++;
		if (input_file.eof() == true) 
			break;
		measures.push_back(mis);
	}

	input_file.close();

	TCanvas* c = new TCanvas();
	int nBin = measures.size();
	TH1F* h1 = new TH1F("h1", graph_name, nBin, 0, 2048);

	for(int i = 0; i < measures.size(); i++)
		h1 -> SetBinContent(i+1, measures.at(i));

	h1 -> GetXaxis() -> SetTitle(x_axis);
	h1 -> GetYaxis() -> SetTitle(y_axis);
	h1 -> SetTitle(graph_name);

	double p0  = 1200; 
	double p1 = guess;    
	double p2  = 15 ;       
	double max = 0; 

	TF1* model = new TF1("model", "gaus(0) + pol1()");
	model -> SetParameter (0, p0); 
	model -> SetParameter (1, p1);
	model -> SetParameter (2, p2);
	model -> SetParameter (3, 50);
	model -> SetParameter (4, 50);
	max = p1;

	model -> SetRange (max - 300, max + 300);  // maybe useless
	model -> SetLineColor (kRed);

	h1 -> GetXaxis() -> SetRangeUser(max - 300, max + 300);
	h1 -> Fit("model", "R");

	gStyle -> SetOptFit(1111);
	h1 -> Draw();
	model -> Draw("same");

	TFitResultPtr r = h1 -> Fit("model", "S");  
	double amp = r -> Parameter(0);
	double centr = r -> Parameter(1);
	double sigmaG = r -> Parameter(2);
	double err_amp = r -> ParError(0);
	double err_mu = r -> ParError(1);
	double err_sigma = r -> ParError(2);

	std::vector<double> parameters;
	parameters.push_back(amp);
	parameters.push_back(centr);
	parameters.push_back(sigmaG);
	parameters.push_back(err_amp);
	parameters.push_back(err_mu);
	parameters.push_back(err_sigma);

	return parameters;
}

std::vector<double> linFit(const char *file_name, const char *x_axis, const char *y_axis, const char *graph_name)
{
	TGraphErrors * graph = new TGraphErrors(file_name);
	TCanvas * c1 = new TCanvas();

    graph -> GetXaxis() -> SetTitle(x_axis);
    graph -> GetYaxis() -> SetTitle(y_axis);
    graph -> SetTitle(graph_name);
   
	TF1* model = new TF1("model", "pol2");
	model -> SetParameter (0, 0); 
	model -> SetParameter (1, 0.3);
	model -> SetParameter (2, 0);
	model -> SetLineColor (kRed);

    graph -> Fit("model", "R");
	graph -> SetLineWidth(0);
	graph -> SetMarkerStyle(8);
    graph -> Draw();
	model -> Draw("same");
    gStyle -> SetOptFit(1111);

	TFitResultPtr r = graph -> Fit("pol2", "S");  // pol2 is ax2 + bx + c
	double a = r -> Parameter(0);
	double b = r -> Parameter(1);
	double c = r -> Parameter(2);
	double err_a = r -> ParError(0);
	double err_b = r -> ParError(1);
	double err_c = r -> ParError(2);

	std::vector<double> parameters;
	parameters.push_back(a);
	parameters.push_back(b);
	parameters.push_back(c);
	parameters.push_back(err_a);
	parameters.push_back(err_b);
	parameters.push_back(err_c);

	return parameters;
}

/*
std::vector<double> comptonFit(const char* file_name, const char *x_axis, const char *y_axis, const char *graph_name)
{
	TGraphErrors * graph = new TGraphErrors(file_name);
   
    graph -> GetXaxis() -> SetTitle(x_axis);
    graph -> GetYaxis() -> SetTitle(y_axis);
    graph -> SetTitle(graph_name);
   
    graph -> Fit("pol2");
    graph -> Draw();
    gStyle -> SetOptFit(1111);

	TFitResultPtr r = graph -> Fit("pol2", "S");  // pol2 is ax2 + bx + c
	double a = r -> Parameter(0);
	double b = r -> Parameter(1);
	double c = r -> Parameter(2);
	double err_a = r -> ParError(0);
	double err_b = r -> ParError(1);
	double err_c = r -> ParError(2);

	std::vector<double> parameters;
	parameters.push_back(a);
	parameters.push_back(b);
	parameters.push_back(c);
	parameters.push_back(err_a);
	parameters.push_back(err_b);
	parameters.push_back(err_c);

	return parameters;
}

std::vector<double> kleinNishinaFit(const char* file_name, const char *x_axis, const char *y_axis, const char *graph_name)
{
	TGraphErrors * graph = new TGraphErrors(file_name);
   
    graph -> GetXaxis() -> SetTitle(x_axis);
    graph -> GetYaxis() -> SetTitle(y_axis);
    graph -> SetTitle(graph_name);
   
    graph -> Fit("pol2");
    graph -> Draw();
    gStyle -> SetOptFit(1111);

	TFitResultPtr r = graph -> Fit("pol2", "S");  // pol2 is ax2 + bx + c
	double a = r -> Parameter(0);
	double b = r -> Parameter(1);
	double c = r -> Parameter(2);
	double err_a = r -> ParError(0);
	double err_b = r -> ParError(1);
	double err_c = r -> ParError(2);

	std::vector<double> parameters;
	parameters.push_back(a);
	parameters.push_back(b);
	parameters.push_back(c);
	parameters.push_back(err_a);
	parameters.push_back(err_b);
	parameters.push_back(err_c);

	return parameters;

	return;
}

*/


#endif 

