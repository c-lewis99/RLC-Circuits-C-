//Implementation of classes and functions declared in 'classes.h'
#include <iostream>
#include <ostream> 
#include <limits>
#include <memory>
#include <complex>
#include "classes.h"


//A template to check whether an entered input (normally a double) is within a specified range 
template<class type> type valueCheck(type low, type high)
{
    type x;
    std::cin>>x;
    while (x<low || x>high || !std::cin.good()){
        std::cout<<"Please enter a number between "<<low<<" and "<<high<<": "<<std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin>>x;
    }
    std::cout<<'\n';
    return x;
}

// Used in the circuit printing function
void print_dash(int x)
{
    for (int i{}; i<x; i++){
        std::cout<<"-";
    }
}

//Resistors

resistor::resistor (double r, double l, double c){ resistance=r; inductance=l; capacitance=c;}

double resistor::get_ang_freq(){return angular_frequency;}

void resistor::set_ang_freq(double w){angular_frequency=w;}

double resistor::get_mod(){return abs(impedance);}

double resistor::get_phase(){return arg(impedance);}

//calculates the impedance of a real resistor with parasitic inductance/capacitance
std::complex<double> resistor::get_impedance()
{
    std::complex<double>a{resistance, angular_frequency*inductance};
    std::complex<double>b{1-angular_frequency*angular_frequency*capacitance*inductance, angular_frequency*resistance*capacitance};
    impedance=a/b;
    return impedance;
}

std::string resistor::type(){component_type = "R"; return component_type;}

//Used to modify the class data
void resistor::changeValues()
{
    std::cout<<"Enter the new resistance: "<<std::endl;
    resistance=valueCheck<double>(1e-6, 1e9);
    std::cout<<"Enter the new parasitic inductance: "<<std::endl;
    inductance=valueCheck<double>(0, 10);
    std::cout<<"Enter the new parasitic capacitance: "<<std::endl;
    capacitance=valueCheck<double>(0, 10);
}

void resistor::showImpedance()
{
    get_impedance();
    std::cout<<"Magnitude = "<<get_mod()<<" Ohms"<<std::endl;
    std::cout<<"Phase = "<<get_phase()<<" Radians"<<std::endl;
}

void resistor::info()
{
    std::cout<<"Real resistor, resistance = "<<resistance<<" Ohms, parasitic inductance = "<<inductance
    <<" Henrys, parasitic capacitance =  "<<capacitance<<" Farads"<<std::endl;
}

//Ideal resistor
//Constructs a real resistor with zero parasitic inductance/capacitance
ideal_resistor::ideal_resistor(double r):resistance(r), resistor(r, 0, 0){}

std::complex<double> ideal_resistor::get_impedance()
{
    impedance = std::complex<double>(resistance, 0);
    return impedance;
}

std::string ideal_resistor::type(){component_type = "r"; return component_type;}

void ideal_resistor::changeValues()
{
    std::cout<<"Enter the new resistance: "<<std::endl;
    resistance=valueCheck<double>(1e-6, 1e9);
}

void ideal_resistor::info(){std::cout<<"Ideal resistor, resistance = "<<resistance<<" Ohms"<<std::endl;}


//Inductors (similar functions to those of the resistor)

inductor::inductor (double r, double l, double c): resistance(r), inductance(l), capacitance(c){}

double inductor::get_ang_freq(){return angular_frequency;}

void inductor::set_ang_freq(double w){angular_frequency=w;}

double inductor::get_mod(){return abs(impedance);}

double inductor::get_phase(){return arg(impedance);}

std::complex<double> inductor::get_impedance()
{
    std::complex<double>a{resistance, angular_frequency*inductance};
    std::complex<double>b{1-angular_frequency*angular_frequency*capacitance*inductance, angular_frequency*resistance*capacitance};
    impedance=a/b;
    return impedance;
}

std::string inductor::type(){component_type = "L"; return component_type;}

void inductor::changeValues()
{
    std::cout<<"Enter the new inductance: "<<std::endl;
    inductance=valueCheck<double>(1e-6, 1e3);
    std::cout<<"Enter the new parasitic resistance: "<<std::endl;
    resistance=valueCheck<double>(0, 10);
    std::cout<<"Enter the new parasitic capacitance: "<<std::endl;
    capacitance=valueCheck<double>(0, 10);
}

void inductor::showImpedance()
{
        std::cout<<"Magnitude = "<<get_mod()<<" Ohms"<<std::endl;
        std::cout<<"Phase = "<<get_phase()<<" Radians"<<std::endl;
}

void inductor::info()
{
    std::cout<<"Real inductor, inductance = "<<inductance<<" Henrys, parasitic resistance = "<<resistance
    <<" Ohms, parasitic capacitance =  "<<capacitance<<" Farads"<<std::endl;
}

//Ideal inductor
ideal_inductor::ideal_inductor(double l):inductance(l), inductor(0, l, 0){}

std::complex<double> ideal_inductor::get_impedance()
{
    impedance = std::complex<double>(0, angular_frequency*inductance);
    return impedance;
}

std::string ideal_inductor::type(){component_type = "l"; return component_type;}

void ideal_inductor::changeValues()
{
    std::cout<<"Enter the new inductance: "<<std::endl;
    inductance=valueCheck<double>(1e-6, 1e3);
}

void ideal_inductor::info(){std::cout<<"Ideal inductor, inductance = "<<inductance<<" Henrys"<<std::endl;}


//Capacitors

capacitor::capacitor(double r, double l, double c): resistance(r), inductance(l), capacitance(c){}

double capacitor::get_ang_freq(){return angular_frequency;}

void capacitor::set_ang_freq(double w){angular_frequency=w;}

double capacitor::get_mod(){return abs(impedance);}

double capacitor::get_phase(){return arg(impedance);}

std::complex<double> capacitor::get_impedance()
{
    std::complex<double> temp(resistance, angular_frequency*inductance-1/(angular_frequency*capacitance));
    impedance=temp;
    return impedance;
}

std::string capacitor::type(){component_type = "C"; return component_type;}

void capacitor::changeValues()
{
    std::cout<<"Enter the new capacitance: "<<std::endl;
    capacitance=valueCheck<double>(1e-12, 1e4);
    std::cout<<"Enter the new parasitic inductance: "<<std::endl;
    inductance=valueCheck<double>(0, 10);
    std::cout<<"Enter the new parasitic resistance: "<<std::endl;
    resistance=valueCheck<double>(0, 10);
    }

void capacitor::showImpedance()
{
    std::cout<<"Magnitude = "<<get_mod()<<" Ohms"<<std::endl;
    std::cout<<"Phase = "<<get_phase()<<" Radians"<<std::endl;
}

void capacitor::info()
{
        std::cout<<"Real capacitor, capacitance = "<<capacitance<<" Farads, parasitic inductance = "<<inductance
        <<" Henrys, parasitic resistance =  "<<resistance<<" Ohms"<<std::endl;
}

//Ideal capacitor
ideal_capacitor::ideal_capacitor(double c):capacitance(c), capacitor(0, 0, c){}

std::complex<double> ideal_capacitor::get_impedance()
{
    impedance = std::complex<double>(0, -1/(angular_frequency*capacitance));
    return impedance;
}

std::string ideal_capacitor::type(){component_type = "c"; return component_type;}

void ideal_capacitor::changeValues()
{
    std::cout<<"Enter the new capacitance: "<<std::endl;
    capacitance=valueCheck<double>(1e-12, 1e4);
}

void ideal_capacitor::info()
{
    get_impedance();
    std::cout<<"Ideal capacitor, capacitance = "<<capacitance<<" Farads"<<std::endl;
}


//Circuit member functions
//Constructed with a base component, voltage and frequency 
circuit::circuit(std::shared_ptr<component> c, double v, double f)
{
    components.push_back(c);
    voltage = v;
    angular_frequency = f*2*M_PI;
    c->set_ang_freq(angular_frequency); //sets the angular frequency of the component 
}

void circuit::set_ang_freq(double w){angular_frequency=w;}

double circuit::get_ang_freq(){return angular_frequency;}

double circuit::get_mod(){return abs(impedance);}

double circuit::get_phase(){return arg(impedance);}

std::complex<double> circuit::get_impedance(){return impedance;}

std::string circuit::type(){component_type = "Circuit"; return component_type;}

void circuit::changeValues(){;}

void circuit::showImpedance()
{
    std::cout<<"Total Impedance = "<<impedance<<std::endl;
    std::cout<<"Magnitude = "<<get_mod()<<" Ohms"<<std::endl;
    std::cout<<"Phase = "<<get_phase()<<" Radians"<<std::endl;
}

void circuit::info()
{
    std::cout<<"Voltage = "<<voltage<<" Volts"<<std::endl;
    std::cout<<"Angular frequency = "<<angular_frequency<<" Rads/s"<<std::endl;
    std::cout<<"Total impedance = "<<impedance<<std::endl;
}

double circuit::get_voltage(){return voltage;}

//Adds the impedances of the components in the circuit line
std::complex<double> circuit::impedance_calc()
{
    std::complex<double> temp{0, 0};
    impedance=temp;
        for (auto iterate:components){
            impedance=impedance + iterate->get_impedance();
        }
    return impedance;
}

//Adds components to component vector and sets the angular freqeuncy
void circuit::addcomponent(std::shared_ptr<component> c)
{
    components.push_back(c);
    c->set_ang_freq(angular_frequency);
}

//This function prints a scaled line of components
void circuit::print_line(int max_components)
{
    int edges = max_components*3+2-(components.size()-1)*3-1 ;//computes the total number of dashes '-' at the edges of the line 
    int dash_number = max_components*3+2 - components.size(); //computes the total number of dashes between the components
 
    if (edges%2==0){ //edges even
        print_dash(edges/2); 
        for (int i{}; i<components.size()-1; i++){
            std::cout<<components[i]->type();
            print_dash(2); // prints 2 dashes in between each component
        }
        std::cout<<components[components.size()-1]->type();
        print_dash(edges/2);
        std::cout<<std::endl;
    }

    if (edges%2!=0){ //edges odd
        double edges_double = edges;
        print_dash(edges_double/2+0.5);
        for (int i{}; i<components.size()-1; i++){
            std::cout<<components[i]->type();
            print_dash(2);
        }
        std::cout<<components[components.size()-1]->type();
        print_dash(edges_double/2-0.5);
        std::cout<<std::endl;
    }
}
void circuit::componentInfo()
{
    double I = get_mod();
    for (auto iterate: components){
        iterate->info();
        iterate->showImpedance();
        std::cout<<"Voltage drop = "<<voltage*iterate->get_mod()/I<<" Volts"<<std::endl;
        std::cout<<" "<<std::endl;
    }
}

int circuit::component_number()
{
    return components.size(); //number of components on the line
}

