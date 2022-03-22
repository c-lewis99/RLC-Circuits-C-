//The main body of the project, this includes all relevant functions and code for the user interface
#include <iostream>
#include <ostream> 
#include <cmath>
#include <sstream>
#include <iomanip>
#include <array>
#include <vector>
#include <limits>
#include <memory>
#include <complex>
#include "classes.h"


void print_vertical_wire(int x)
{
    std::cout<<"|";
    for (int i{}; i<x-2; i++){
        std::cout<<" ";
    }
    std::cout<<"|"<<std::endl;
}

template<class type> type maxValue(type a, type b) {return (a>b)? a:b;}

//Calculates the maximum number of components on a line 
int max_on_line(std::vector<std::shared_ptr<circuit>> circ)
{
    int max{};
    for (int i{}; i<circ.size(); i++){
        max = maxValue<int>(max, circ[i]->component_number());
    }
    return max;
}

//prints the complete circuit diagram
void printCircuit(std::vector<std::shared_ptr<circuit>> circ)
{
    int max_component = max_on_line(circ);
    int line_length = max_component*3+2; //total number of characters on each line
    std::cout<<"Diagram: \n"<<std::endl;

    if ((line_length-1)%2==0){ //symmetric case
        print_dash(line_length/2);
        std::cout<<"V"; //power source
        print_dash(line_length/2); 
        std::cout<<std::endl;
    }
    else{
        double line_length_double = line_length; //int to double for use in print_dash function
        print_dash(line_length_double/2+0.5);
        std::cout<<"V";
        print_dash(line_length_double/2-0.5);
        std::cout<<std::endl;
    }
    //prints line by line
    for (int i{}; i<circ.size(); i++){
        print_vertical_wire(line_length);
        circ[i]->print_line(max_component);
    }
}
//calculates total impedance of circuit and prints information
void printImpendance(std::vector<std::shared_ptr<circuit>> circ)
{
    std::complex<double> impedance;
    std::complex<double> temp(0,0);
    std::complex<double> real_unit (1, 0);
    for (auto iterate:circ){
        temp = temp + real_unit/iterate->impedance_calc();
    }
    impedance = real_unit/temp;
    std::cout<<"Voltage = "<<circ[0]->get_voltage()<<" Volts"<<std::endl;
    std::cout<<"Frequency = "<<circ[0]->get_ang_freq()/(2*M_PI)<<" Hz"<<std::endl;
    std::cout<<"Impendance:  magnitude = "<<abs(impedance)<<" Ohms, phase = "<<arg(impedance)<<" radians"<<std::endl;
    std::cout<<"Circuit current = "<<circ[0]->get_voltage()/abs(impedance)<<" Amps "<<std::endl;
}

//user validation function
std::string yes_no()
{
    std::string input;
    std::cin>>input;
    while(input!="y" && input!="n"){
        std::cout<<"Please enter 'y' or 'n': "<<std::endl;
        std::cin.clear();
        std::cin.ignore();
        std::cin>>input;
    }
    std::cout<<'\n';
    return input;
}

//returns true if entered string is an integer only
bool intCheck(std::string &str)
{
    for (char const &ch: str){
        if (std::isdigit(ch) == 0){
            return false;
        }
    }
    return true;
}

//used in menu selection, checks whether user entry is an integer between 2 values
int checkMenu(int low, int high)
{
    using namespace std;
    int x{};
    string str;
    cin.clear();
    cin>>str;
    std::stringstream ss(str);
    ss>>x; //attempts to convert stringstream to integer
    while(x<low||x>high||intCheck(str)==false){
        cout<<"Please enter an integer between "<<low<<" and "<<high<<": "<<endl;
        cin.clear();
        cin>>str;
        std::stringstream ss(str);
        ss >> x;
    }
    cout<<"\n";
    return x;
}

int enter_component_number()
{
    using namespace std;
    cout<<"There are 6 components to choose from: "<<endl;
    cout<<"1. Resistor"<<endl;
    cout<<"2. Ideal resistor"<<endl;
    cout<<"3. Inductor"<<endl;
    cout<<"4. Ideal inductor"<<endl;
    cout<<"5. Capacitor"<<endl;
    cout<<"6. Ideal Capacitor"<<endl;
    cout<<"Please enter a corresponding number from 1-6 to select a component: "<<endl;

    int component_number = checkMenu(1, 6);
    return component_number;
}

//returns a pointer to a created component
std::shared_ptr<component> create_component()
{
    using namespace std;
    int choice = enter_component_number();
    std::shared_ptr<component> temp;

    double r, l, c;
    switch(choice){
        case 1:{
            cout<<"You have chosen a resistor"<<endl;
            cout<<"Enter the resistance: "<<endl;
            r=valueCheck<double>(1e-6, 1e9);
            cout<<"Enter the parasitic inductance:"<<endl;
            l=valueCheck<double>(0, 10);
            cout<<"Enter the parasitic capacitance:"<<endl;
            c=valueCheck<double>(0, 10);
            temp=make_shared<resistor>(r, l, c);
            break;
        }
        case 2:{
            cout<<"You have chosen an ideal resistor"<<endl;
            cout<<"Enter the resistance: "<<endl;
            r=valueCheck<double>(1e-6, 1e9);
            temp=make_shared<ideal_resistor>(r);
            break;
        }
        case 3:{
            cout<<"You have chosen a inductor"<<endl;
            cout<<"Enter the inductance: "<<endl;
            l=valueCheck<double>(1e-6, 1e4);
            cout<<"Enter the parasitic resistance:"<<endl;
            r=valueCheck<double>(0, 10);
            cout<<"Enter the parasitic capacitance:"<<endl;
            c=valueCheck<double>(0, 10);
            temp=make_shared<inductor>(r, l, c);
            break;
        }
        case 4:{
            cout<<"You have chosen an ideal inductor"<<endl;
            cout<<"Enter the inductance: "<<endl;
            l=valueCheck<double>(1e-6, 1e4);
            temp=make_shared<ideal_inductor>(l);
            break;
        }
        case 5:{
            cout<<"You have chosen a capacitor"<<endl;
            cout<<"Enter the capacitance: "<<endl;
            c=valueCheck<double>(1e-12, 1e4);
            cout<<"Enter the parasitic resistance:"<<endl;
            r=valueCheck<double>(0, 10);
            cout<<"Enter the parasitic inductance:"<<endl;
            l=valueCheck<double>(0, 10);
            temp=make_shared<capacitor>(r, l, c);
            break;
        }
        case 6:{
            cout<<"You have chosen an ideal capacitor"<<endl;
            cout<<"Enter the capacitance: "<<endl;
            c=valueCheck<double>(1e-12, 1e4);
            temp=make_shared<ideal_capacitor>(c);
            break;
        }
    }
    return temp;
}

//adds created component to library
void create_component_menu(std::vector<std::shared_ptr<component>> &container)
{
    using namespace std;
    std::string input{"y"};
    while(input=="y"){
            container.push_back(create_component());
            cout<<"Would you like to add another component?[y/n]"<<endl;
            input = yes_no();
        }  
}

void display_components(std::vector<std::shared_ptr<component>> &container)
{
    if (container.size()==1){
        std::cout<<"There is 1 component in the library: "<<std::endl;
        std::cout<<"1. ";
        container[0]->info();
    }
    else{
        std::cout<<"There are "<<container.size()<<" components in the library:"<<std::endl;
        int i{};
        for (auto iterate:container){
            std::cout<<i+1<<". ";
            iterate->info();
            i++;
        }
    }
}

void modify_components(std::vector<std::shared_ptr<component>> &container)
{
    using namespace std;
    if (container.size()==0){
        cout<<"There are no components. Please create some first."<<endl;
    }
    else{
        std::string input{"y"};
        while(input=="y"){
            display_components(container);
            cout<<"Enter the component number you wish to modify: "<<endl;
            int component_number = checkMenu(1, container.size());
            container[component_number-1]->changeValues();
            container[component_number-1]->get_impedance();
            cout<<"Would you like to modify another component?[y/n]"<<endl;
            input = yes_no();
        }
    }
}

void delete_data(std::vector<std::shared_ptr<component>> &container, std::vector<std::shared_ptr<circuit>> &circ)
{
    std::cout<<"Are you sure you wish to proceed? [y/n]"<<std::endl;
    std::string answer = yes_no();
    if (answer=="y"){
        container.clear();
        circ.clear();
        std::cout<<"Data deleted. "<<std::endl;
    }
}

//creates/overwrites circuit
void create_circuit(std::vector<std::shared_ptr<component>> &container, std::vector<std::shared_ptr<circuit>> &circ)
{
    using namespace std;
    if (circ.size()>0){
        cout<<"Please be aware that this will overwrite the existing circuit. Do you wish to proceed?[y/n]"<<endl;
        std::string answer = yes_no();
        if (answer=="y"){
            circ.clear();
            cout<<"Circuit deleted"<<endl;
            create_circuit(container, circ); //recursion - will now skip this if statement since circuit has now been deleted
        }
        else {
            cout<<"Original circuit remains."<<endl;
        }
    }

    else { 
        cout<<"Please enter the voltage of the power source: "<<endl;
        double voltage = valueCheck<double>(1e-6, 1e9);
        cout<<"Please enter the frequency of the power source: "<<endl;
        double circuit_freq = valueCheck<double>(1e-3, 1e9);

        if (container.size()==1){
            circ.push_back(make_shared<circuit>(container[0], voltage, circuit_freq)); //creates an instance of the circuit class the circuit vector
            cout<<"Circuit created!"<<endl;
        }

        else{
            display_components(container);
            cout<<"Enter the base component number: "<<endl;
            int component_number = checkMenu(1, container.size());
            circ.push_back(make_shared<circuit>(container[component_number-1], voltage, circuit_freq));
            cout<<"Circuit created! Now add some more components."<<endl;
        }
    }
}


void circuit_details(std::vector<std::shared_ptr<circuit>> &circ){
    using namespace std;

    if (circ.size()==0){
        cout<<"Please create a circuit first!"<<endl;
    }
    else{
        cout<<"Circuit:"<<endl;
        printImpendance(circ);
        cout<<"\n";
        printCircuit(circ);
        cout<<"\n";
        cout<<"Would you like to view advanced component info? [y/n]"<<endl;
        string answer = yes_no();
        if (answer=="y"){
            int line =1;
            for (auto iterate: circ){
                if (circ.size()>1){
                    cout<<"Line "<<line<<" info: "<<endl;
                    cout<<"Current through line = "<<iterate->get_voltage()/iterate->get_mod()<<" Amps \n"<<endl;
                }
                iterate->componentInfo();
                line+=1;
            }
        }
    }

}

void addSeries(std::vector<std::shared_ptr<component>> &container, std::vector<std::shared_ptr<circuit>> &circ)
{
    using namespace std;

    if (circ.size()==0){
        cout<<"Please create a circuit first! "<<endl;
    }
    else{

        cout<<"Select a component to add to the circuit: "<<endl;
        display_components(container);
        int number = checkMenu(1, container.size());
        
        if (circ[0]->component_number()==1){
            cout<<"This will be added in series to the base component."<<endl;
            circ[0]->addcomponent(container[number-1]); //adds the component to the component vector of the line
        }
        else if(circ.size()==1 && circ[0]->component_number()>1){
            cout<<"This will be added in series to line 1."<<endl;
            circ[0]->addcomponent(container[number-1]);
        }
        else{
            cout<<"Which line would you like to add this to? [1-"<<circ.size()<<"] "<<endl;
            int line_number = checkMenu(1, circ.size());
            circ[line_number-1]->addcomponent(container[number-1]);
            cout<<"Added in series to line "<<line_number<<endl;
        }
    }
}

void addParallel(std::vector<std::shared_ptr<component>> &container, std::vector<std::shared_ptr<circuit>> &circ)
{
    using namespace std;

    if (circ.size()==0){
        cout<<"Please create a circuit first! "<<endl;
    }

    else{
        cout<<"Select a component to add to the circuit: "<<endl;
        display_components(container);
        int number = checkMenu(1, container.size());

        if (circ[0]->component_number()==1){
            cout<<"This will be added in parallel to the base component."<<endl;
            circ.push_back(make_shared<circuit>(container[number-1], circ[0]->get_voltage(), circ[0]->get_ang_freq()/(2*M_PI)));
        }
        else {
            cout<<"This will be added in parallel to line "<<circ.size()<<" ."<<endl;
            circ.push_back(make_shared<circuit>(container[number-1], circ[0]->get_voltage(), circ[0]->get_ang_freq()/(2*M_PI)));
        } //creates a new circuit line in the circuit vector
    }
}

void programmeInfo()
{
    using namespace std;
    cout<<"Programme info: "<<endl;
    cout<<"This programme gives the user the ability to construct simple series/parallel circuits with a number of different components."<<endl;
    cout<<"There are three types of component (resistor, inductor, capacitor) each with a real and ideal option."<<endl;
    cout<<"The real components have parasitic effects. For example, a real resistor can be modelled as a mini RLC circuit with parasitic inductance and capacitance."<<endl;
    cout<<"The component library is a vector containing pointers to components; for this reason the same component can be used multiple times within a circuit."<<endl;
    cout<<"The main circuit is created line by line. Components can be added to a specific line, either in series or parallel."<<endl;
    cout<<"The circuit diagram consists of the power source 'V' connnected to the various components; real components are uppercase, whilst "<<endl;
    cout<<"ideal components are lowercase. For example, a real inductor is printed as 'L'. "<<endl;
    cout<<"The programme will also print out the magnitude and phase of the total impedance of the circuit, as well as for the individual components"<<endl;
    cout<<"contained within the circuit. "<<endl;
    cout<<"It will also calculate the current and voltage drop through each component."<<endl;
    cout<<"Assume all quantities are in S.I units.\n"<<endl;
}

//the user interface, utilising a switch statement
int menu()
{
    using namespace std;
    std::vector<shared_ptr<component>> component_container;
    std::vector<shared_ptr<circuit>> circuit_container;
    int menu_choice{};

    while(menu_choice!=10){
        cout<<"\n";
        cout<<"Menu"<<endl;
        cout<<"----"<<endl;
        cout<<"What would you like to do?"<<endl;
        cout<<"     1. View programme information"<<endl;
        cout<<"     2. Create components"<<endl;
        cout<<"     3. Modify components"<<endl;
        cout<<"     4. View component information"<<endl;
        cout<<"     5. Create circuit"<<endl;
        cout<<"     6. Add component in series"<<endl;
        cout<<"     7. Add component in parallel"<<endl;
        cout<<"     8. View circuit information"<<endl;
        cout<<"     9. Delete all data"<<endl;
        cout<<"     10. Terminate the programme"<<endl;
        cout<<"\n";

        menu_choice = checkMenu(1, 10);
       
        switch(menu_choice){ 
            case 1:{
                programmeInfo();
                break;
            }

            case 2:{
                create_component_menu(component_container);
                break;
            }

            case 3:{
                modify_components(component_container);
                break;
            }

            case 4:{
                display_components(component_container);
                break;
            }

            case 5:{
                if (component_container.size()==0){
                    cout<<"Please create some components first!"<<endl;
                }
                else{
                    create_circuit(component_container, circuit_container);
                }
                break;
            }

            case 6:{
                addSeries(component_container, circuit_container);
                break;
            }

            case 7:{
                addParallel(component_container, circuit_container);
                break;
            }

            case 8:{
                circuit_details(circuit_container);
                break;
            }

            case 9:{
                delete_data(component_container, circuit_container);
                break;
            }
        }
    }
    cout<<"Thank you for using this programme. "<<endl;
    return 0;
}

int main()
{  
    menu();

    return 0;
}