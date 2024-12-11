#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <random>
#include "FiniteFunctions.h"
#include <filesystem> //To check extensions in a nice way

#include "gnuplot-iostream.h" //Needed to produce plots (not part of the course) 

using std::filesystem::path;

//Empty constructor
FiniteFunction::FiniteFunction(){
  m_RMin = -5.0;
  m_RMax = 5.0;
  this->checkPath("DefaultFunction");
  m_Integral = NULL;
}

//initialised constructor
FiniteFunction::FiniteFunction(double range_min, double range_max, std::string outfile){
  m_RMin = range_min;
  m_RMax = range_max;
  m_Integral = NULL;
  this->checkPath(outfile); //Use provided string to name output files
}

//Plots are called in the destructor
//SUPACPP note: They syntax of the plotting code is not part of the course
FiniteFunction::~FiniteFunction(){
  Gnuplot gp; //Set up gnuplot object
  this->generatePlot(gp); //Generate the plot and save it to a png using "outfile" for naming 
}

/*
###################
//Setters
###################
*/ 
void FiniteFunction::setRangeMin(double RMin) {m_RMin = RMin;};
void FiniteFunction::setRangeMax(double RMax) {m_RMax = RMax;};
void FiniteFunction::setOutfile(std::string Outfile) {this->checkPath(Outfile);};

/*
###################
//Getters
###################
*/ 
double FiniteFunction::rangeMin() {return m_RMin;};
double FiniteFunction::rangeMax() {return m_RMax;};

/*
###################
//Function eval
###################
*/ 
double FiniteFunction::invxsquared(double x) {return 1/(1+x*x);};
double FiniteFunction::callFunction(double x) {return this->invxsquared(x);}; //(overridable)

/*
###################
Integration by hand (output needed to normalise function when plotting)
###################
*/ 
double FiniteFunction::integrate(int Ndiv){ //private
  double x_inc = (this->rangeMax() - this->rangeMin()) / (Ndiv - 1);

  // implement trapezium rule
  double integral = 0.0;
  for (int i=0; i<Ndiv; i++) {
    double val = this->callFunction(this->rangeMin() + i * x_inc);
    if (i==0 || i==Ndiv-1) {
      val *= 0.5;
    }
    integral += val;
  }
  integral *= x_inc;

  return integral;  
}
double FiniteFunction::integral(int Ndiv) { //public
  if (Ndiv <= 0){
    std::cout << "Invalid number of divisions for integral, setting Ndiv to 1000" <<std::endl;
    Ndiv = 1000;
  }
  if (m_Integral == NULL || Ndiv != m_IntDiv){
    m_IntDiv = Ndiv;
    m_Integral = this->integrate(Ndiv);
    return m_Integral;
  }
  else return m_Integral; //Don't bother re-calculating integral if Ndiv is the same as the last call
}

/*
###################
//Helper functions 
###################
*/
// Generate paths from user defined stem
void FiniteFunction::checkPath(std::string outfile){
 path fp = outfile;
 m_FunctionName = fp.stem(); 
 m_OutData = m_FunctionName+".data";
 m_OutPng = m_FunctionName+".png";
}

//Print (overridable)
void FiniteFunction::printInfo(){
  std::cout << "rangeMin: " << m_RMin << std::endl;
  std::cout << "rangeMax: " << m_RMax << std::endl;
  std::cout << "integral: " << m_Integral << ", calculated using " << m_IntDiv << " divisions" << std::endl;
  std::cout << "function: " << m_FunctionName << std::endl;
}

/*
###################
//Plotting
###################
*/

//Hack because gnuplot-io can't read in custom functions, just scan over function and connect points with a line... 
void FiniteFunction::plotFunction(){
  m_function_scan = this->scanFunction(10000);
  m_plotfunction = true;
}

//Transform data points into a format gnuplot can use (histogram) and set flag to enable drawing of data to output plot
//set isdata to true (default) to plot data points in black, set to false to plot sample points in blue
void FiniteFunction::plotData(std::vector<double> &points, int Nbins, bool isdata){
  if (isdata){
    m_data = this->makeHist(points,Nbins);
    m_plotdatapoints = true;
  }
  else{
    m_samples = this->makeHist(points,Nbins);
    m_plotsamplepoints = true;
  }
}


/*
  #######################################################################################################
  ## SUPACPP Note:
  ## The three helper functions below are needed to get the correct format for plotting with gnuplot
  ## In theory you shouldn't have to touch them
  ## However it might be helpful to read through them and understand what they are doing
  #######################################################################################################
 */

//Scan over range of function using range/Nscan steps (just a hack so we can plot the function)
std::vector< std::pair<double,double> > FiniteFunction::scanFunction(int Nscan){
  std::vector< std::pair<double,double> > function_scan;
  double step = (m_RMax - m_RMin)/(double)Nscan;
  double x = m_RMin;
  //We use the integral to normalise the function points
  if (m_Integral == NULL) {
    std::cout << "Integral not set, doing it now" << std::endl;
    this->integral(Nscan);
    std::cout << "integral: " << m_Integral << ", calculated using " << Nscan << " divisions" << std::endl;
  }
  //For each scan point push back the x and y values 
  for (int i = 0; i < Nscan; i++){
    function_scan.push_back( std::make_pair(x,this->callFunction(x)/m_Integral));
    x += step;
  }
  return function_scan;
}

//Function to make histogram out of sampled x-values - use for input data and sampling
std::vector< std::pair<double,double> > FiniteFunction::makeHist(std::vector<double> &points, int Nbins){

  std::vector< std::pair<double,double> > histdata; //Plottable output shape: (midpoint,frequency)
  std::vector<int> bins(Nbins,0); //vector of Nbins ints with default value 0 
  int norm = 0;
  for (double point : points){
    //Get bin index (starting from 0) the point falls into using point value, range, and Nbins
    int bindex = static_cast<int>(floor((point-m_RMin)/((m_RMax-m_RMin)/(double)Nbins)));
    if (bindex<0 || bindex>Nbins){
      continue;
    }
    bins[bindex]++; //weight of 1 for each data point
    norm++; //Total number of data points
  }
  double binwidth = (m_RMax-m_RMin)/(double)Nbins;
  for (int i=0; i<Nbins; i++){
    double midpoint = m_RMin + i*binwidth + binwidth/2; //Just put markers at the midpoint rather than drawing bars
    double normdata = bins[i]/((double)norm*binwidth); //Normalise with N = 1/(Ndata*binwidth)
    histdata.push_back(std::make_pair(midpoint,normdata));
  }
  return histdata;
}

//Function which handles generating the gnuplot output, called in destructor
//If an m_plot... flag is set, the we must have filled the related data vector
//SUPACPP note: They syntax of the plotting code is not part of the course
void FiniteFunction::generatePlot(Gnuplot &gp){

  if (m_plotfunction==true && m_plotdatapoints==true && m_plotsamplepoints==true){
    gp << "set terminal pngcairo\n";
    gp << "set output 'Outputs/png/"<<m_FunctionName<<".png'\n"; 
    gp << "set xrange ["<<m_RMin<<":"<<m_RMax<<"]\n";
    gp << "set style line 1 lt 1 lw 2 pi 1 ps 0\n";
    gp << "plot '-' with linespoints ls 1 title '"<<m_FunctionName<<"', '-' with points ps 2 lc rgb 'blue' title 'sampled data', '-' with points ps 1 lc rgb 'black' pt 7 title 'data'\n";
    gp.send1d(m_function_scan);
    gp.send1d(m_samples);
    gp.send1d(m_data);
  }
  else if (m_plotfunction==true && m_plotdatapoints==true){
    gp << "set terminal pngcairo\n";
    gp << "set output 'Outputs/png/"<<m_FunctionName<<".png'\n"; 
    gp << "set xrange ["<<m_RMin<<":"<<m_RMax<<"]\n";
    gp << "set style line 1 lt 1 lw 2 pi 1 ps 0\n";
    gp << "plot '-' with linespoints ls 1 title '"<<m_FunctionName<<"', '-' with points ps 1 lc rgb 'black' pt 7 title 'data'\n";
    gp.send1d(m_function_scan);
    gp.send1d(m_data);
  }
  else if (m_plotfunction==true && m_plotsamplepoints==true){
    gp << "set terminal pngcairo\n";
    gp << "set output 'Outputs/png/"<<m_FunctionName<<".png'\n"; 
    gp << "set xrange ["<<m_RMin<<":"<<m_RMax<<"]\n";
    gp << "set style line 1 lt 1 lw 2 pi 1 ps 0\n";
    gp << "plot '-' with linespoints ls 1 title '"<<m_FunctionName<<"', '-' with points ps 2 lc rgb 'blue' title 'sampled data'\n";
    gp.send1d(m_function_scan);
    gp.send1d(m_samples);
  }
  else if (m_plotfunction==true){
    gp << "set terminal pngcairo\n";
    gp << "set output 'Outputs/png/"<<m_FunctionName<<".png'\n"; 
    gp << "set xrange ["<<m_RMin<<":"<<m_RMax<<"]\n";
    gp << "set style line 1 lt 1 lw 2 pi 1 ps 0\n";
    gp << "plot '-' with linespoints ls 1 title 'function'\n";
    gp.send1d(m_function_scan);
  }

  else if (m_plotdatapoints == true){
    gp << "set terminal pngcairo\n";
    gp << "set output 'Outputs/png/"<<m_FunctionName<<".png'\n"; 
    gp << "set xrange ["<<m_RMin<<":"<<m_RMax<<"]\n";
    gp << "plot '-' with points ps 1 lc rgb 'black' pt 7 title 'data'\n";
    gp.send1d(m_data);
  }

  else if (m_plotsamplepoints == true){
    gp << "set terminal pngcairo\n";
    gp << "set output 'Outputs/png/"<<m_FunctionName<<".png'\n"; 
    gp << "set xrange ["<<m_RMin<<":"<<m_RMax<<"]\n";
    gp << "plot '-' with points ps 2 lc rgb 'blue' title 'sampled data'\n";
    gp.send1d(m_samples);
  }
}


/*
###################
//Normal Distribution
###################
*/
NormalDist::NormalDist(){
  m_RMin = -5.0;
  m_RMax = 5.0;
  this->checkPath("NormalDist");
  m_Integral = NULL;
}

NormalDist::NormalDist(double mu, double sigma, double range_min, double range_max, std::string outfile) {
  m_RMin = range_min;
  m_RMax = range_max;
  m_Integral = NULL;
  this->checkPath(outfile);
  m_mu = mu;
  m_sigma = sigma;
}

void NormalDist::printInfo() {
  std::cout << "     rangeMin: " << m_RMin << std::endl;
  std::cout << "     rangeMax: " << m_RMax << std::endl;
  std::cout << "     integral: " << m_Integral << ", calculated using " << m_IntDiv << " divisions" << std::endl;
  std::cout << "         mean: " << m_mu << std::endl;
  std::cout << "std deviation: " << m_sigma << std::endl;
  std::cout << "     function: " << m_FunctionName << std::endl;
}

double NormalDist::callFunction(double x) {return this->normal(x);};

double NormalDist::normal(double x) {
  return 1.0 / (m_sigma * pow(2*M_PI, 0.5)) * exp(-0.5*pow((x - m_mu) / m_sigma, 2));
}


/*
###################
//Cauchy-Lorentz
###################
*/
CauchyLorentzDist::CauchyLorentzDist(){
  m_RMin = -5.0;
  m_RMax = 5.0;
  this->checkPath("CauchyLorentzDist");
  m_Integral = NULL;
}

CauchyLorentzDist::CauchyLorentzDist(double x0, double gamma, double range_min, double range_max, std::string outfile) {
  m_RMin = range_min;
  m_RMax = range_max;
  m_Integral = NULL;
  this->checkPath(outfile);
  m_x0 = x0;
  if (gamma > 0) {
    m_gamma = gamma;
  } else {
    std::cout << "gamma must be > 0, choosing gamma = 1 by default" << std::endl;
    m_gamma = 1.0;
  }
}

void CauchyLorentzDist::printInfo() {
  std::cout << "rangeMin: " << m_RMin << std::endl;
  std::cout << "rangeMax: " << m_RMax << std::endl;
  std::cout << "integral: " << m_Integral << ", calculated using " << m_IntDiv << " divisions" << std::endl;
  std::cout << "      x0: " << m_x0 << std::endl;
  std::cout << "   gamma: " << m_gamma << std::endl;
  std::cout << "function: " << m_FunctionName << std::endl;
}

double CauchyLorentzDist::callFunction(double x) {return this->cauchyLorentz(x);};

double CauchyLorentzDist::cauchyLorentz(double x) {
  return 1.0 / (M_PI * m_gamma * (1 + pow((x - m_x0) / m_gamma, 2)));
}


/*
###################
//CrystalBall
###################
*/
CrystalBallDist::CrystalBallDist(){
  m_RMin = -5.0;
  m_RMax = 5.0;
  this->checkPath("CrystalBallDist");
  m_Integral = NULL;
}

CrystalBallDist::CrystalBallDist(double xb, double sigma, double alpha, double n, double range_min, double range_max, std::string outfile) {
  m_RMin = range_min;
  m_RMax = range_max;
  m_Integral = NULL;
  this->checkPath(outfile);
  m_xb = xb;
  m_sigma = sigma;

  if (n > 1) {
    m_n = n;
  } else {
    std::cout << "n must be > 1, choosing n = 2 by default" << std::endl;
    m_n = 2.0;
  }

  if (alpha > 0) {
    m_alpha = alpha;
  } else {
    std::cout << "alpha must be > 0, choosing alpha = 1 by default" << std::endl;
    m_alpha = 1.0;
  }
}

void CrystalBallDist::printInfo() {
  std::cout << "rangeMin: " << m_RMin << std::endl;
  std::cout << "rangeMax: " << m_RMax << std::endl;
  std::cout << "integral: " << m_Integral << ", calculated using " << m_IntDiv << " divisions" << std::endl;
  std::cout << "   x bar: " << m_xb << std::endl;
  std::cout << "   sigma: " << m_sigma << std::endl;
  std::cout << "   alpha: " << m_alpha << std::endl;
  std::cout << "       n: " << m_n << std::endl;
  std::cout << "function: " << m_FunctionName << std::endl;
}

double CrystalBallDist::callFunction(double x) {return this->crystalball(x);};

double CrystalBallDist::crystalball(double x) {
  double C = m_n / abs(m_alpha) * 1/(m_n-1) * exp(-pow(abs(m_alpha), 2) / 2);
  double D = pow(M_PI / 2, 0.5) * (1 + erf(abs(m_alpha) / pow(2, 0.5)));

  double N = 1 / (m_sigma * (C + D));

  if ((x - m_xb) / m_sigma > -m_alpha) {
    return N * exp(-pow(x - m_xb, 2)/(2*pow(m_sigma, 2)));
  } else {
    double A = pow((m_n / abs(m_alpha)), m_n) * exp(-pow(abs(m_alpha), 2) / 2);
    double B = m_n / abs(m_alpha) - abs(m_alpha);
    
    return N * A * pow((B - (x - m_xb) / m_sigma), -m_n);
  }
}


/*
###################
//Sampling
###################
*/
std::vector<double> FiniteFunction::createSampledDataPoints(unsigned int n_points, unsigned int seed, double sigma) {
  std::vector<double> sampled_points;

  // random engine & the initial uniform dist
  std::mt19937 mtEngine(seed);
  std::uniform_real_distribution<double> uniformPDF{this->rangeMin(), this->rangeMax()};
  std::uniform_real_distribution<double> uniformPDF0_1{0.0, 1.0};
  double xi = uniformPDF(mtEngine);

  while (sampled_points.size() < n_points) {
    std::normal_distribution<double> normalPDF{xi, sigma};
    double y = normalPDF(mtEngine);
    
    double A = std::min(this->callFunction(y) / this->callFunction(xi), 1.0);
    double T = uniformPDF0_1(mtEngine);

    if (T < A) {
      sampled_points.push_back(y);
      xi = y;
    }
  }
  return sampled_points;
}
