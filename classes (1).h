
//classes.h
//This header file declares the component and circuit classes 
#include <vector>
#ifndef classes_h
#define classes_h



//Declaration of a template/function used extensively in class member functions
template<class type> type valueCheck(type low, type high);

void print_dash(int x);

// Abstract base component class with pure virtual functions
class component
{
protected: 
    std::complex <double>impedance{};
    double angular_frequency, resistance, inductance, capacitance;
    std::string component_type;
public:
    virtual ~component(){}
    virtual void set_ang_freq(double)=0;
    virtual double get_ang_freq()=0;
    virtual double get_mod()=0;
    virtual double get_phase()=0;
    virtual std::complex <double>get_impedance()=0;
    virtual std::string type()=0;
    virtual void changeValues()=0;
    virtual void showImpedance()=0;
    virtual void info()=0;
};

//Derived real and ideal component classes
class resistor : public component
{
protected:
    double resistance, inductance, capacitance;
public: 
    resistor (double r, double l, double c);
    ~resistor(){}
    void set_ang_freq(double);
    double get_ang_freq();
    double get_mod();
    double get_phase();
    std::complex<double> get_impedance();
    std::string type();
    void changeValues();
    void showImpedance();
    void info();

};

//The ideal components are derived from real ones
class ideal_resistor: public resistor
{
protected:
    double resistance;
    
public:
    ideal_resistor(double r);
    ~ideal_resistor(){}
    std::complex<double> get_impedance();
    std::string type();
    void changeValues();
    void info();
};


class inductor: public component
{
protected:
    double resistance, inductance, capacitance;
public: 
    inductor (double , double , double );
    ~inductor(){}
    void set_ang_freq(double);
    double get_ang_freq();
    double get_mod();
    double get_phase();
    std::complex<double> get_impedance();
    std::string type();
    void changeValues();
    void showImpedance();
    void info();
};

class ideal_inductor: public inductor
{
protected:
    double inductance;
public:
    ideal_inductor(double);
    ~ideal_inductor(){}
    std::complex<double> get_impedance();
    std::string type();
    void changeValues();
    void info();
};

    
class capacitor : public component
{
protected:
    double resistance, inductance, capacitance;
public:
    capacitor (double , double , double );
    ~capacitor(){}
    void set_ang_freq(double);
    double get_ang_freq();
    double get_mod();
    double get_phase();
    std::complex<double> get_impedance();
    std::string type();
    void changeValues();
    void showImpedance();
    void info();
};

class ideal_capacitor: public capacitor
{
protected:
    double capacitance;
public:
    ideal_capacitor(double);
    ~ideal_capacitor(){}
    std::complex<double> get_impedance();
    std::string type();
    void changeValues();
    void info();
};


//Declaration of the circuit class, which also inheritcs from the component class
class circuit: public component
{
protected:
    std::vector<std::shared_ptr<component>> components;
    double voltage, angular_freqency; //contains extra member data used for calculating the circuit impedance
public:
    ~circuit(){}
    circuit()=default;
    circuit(std::shared_ptr<component>, double, double);
    void set_ang_freq(double);
    double get_ang_freq();
    double get_mod();
    double get_phase();
    std::complex<double> get_impedance();
    std::string type();
    void changeValues();
    void showImpedance();
    void info();
    double get_voltage();
    std::complex<double> impedance_calc();
    void addcomponent(std::shared_ptr<component>);
    void print_line(int);
    void componentInfo();
    int component_number();
};

#endif