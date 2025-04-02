#include <string>
#include <memory>
#include <sstream>
#include <iomanip>

using namespace std;

enum ProcessorType
{
    x86,
    x64
};

class IProcessor {
protected:
    string _version;
    ProcessorType _type;
    double _speed;
public:
    IProcessor(const string version, const ProcessorType type, const double speed) :
        _version(version), _type(type), _speed(speed) {}

    virtual string getProcessorInfo() const = 0;
};

class IntelProcessor : public IProcessor
{
    bool _integrated_graphics;
public:
    IntelProcessor(const string version,
                   const ProcessorType type,
                   const double speed,
                   const bool integrated_graphics) :
        IProcessor(version, type, speed),
        _integrated_graphics(integrated_graphics) {}

    string getProcessorInfo() const override
    {
        string type = (_type == ProcessorType::x64) ? "x64" : "x86";
        string integrated_graphics = string(" integrated_graphics: ") + (_integrated_graphics ? "yes" : "no");

        stringstream s;
        s << fixed <<
            "Processor for Intel " <<
            _version << " " <<
            type << " " <<
            setprecision(2) << _speed << integrated_graphics;

        return s.str();
    }
};

class AMDProcessor : public IProcessor
{
    int _tdp_whatt;
public:
    AMDProcessor(const string version,
                 const ProcessorType type,
                 const double speed,
                 const int tdp_whatt) :
        IProcessor(version, type, speed),
        _tdp_whatt(tdp_whatt) {}

    string getProcessorInfo() const override
    {
        string type = (_type == ProcessorType::x64) ? "x64" : "x86";
        string tdp_whatt(" TDP whatt: " + std::to_string(_tdp_whatt) + "W");

        stringstream s;
        s << "Processor for AMD " <<
            _version << " " <<
            type << " " <<
            fixed << setprecision(2) << _speed << tdp_whatt;

        return s.str();
    }
};


class Computer {
    shared_ptr<IProcessor> _processor;
public:
    Computer(shared_ptr<IProcessor> processor) : _processor(processor) {}

    shared_ptr<IProcessor> getProcessor()
    {
        return _processor;
    }
};
