#include <memory>
#include <iostream>
#include <string>

#include "ioc_container.hpp"
using namespace std;

class IAmAThing
{
public:
    virtual ~IAmAThing() {}
    virtual string TestThis() = 0;
};

class IAmTheOtherThing
{
public:
    virtual ~IAmTheOtherThing() {}
    virtual string TheOtherTest() = 0;
};

class TheThing : public IAmAThing
{
public:
    TheThing() {}
    string TestThis() {
        return "TheThing()";
    }
};

class TheOtherThing : public IAmTheOtherThing
{
    std::shared_ptr<IAmAThing> m_thing;
    string m_superInfo;

public:
    TheOtherThing(std::shared_ptr<IAmAThing> thing)
        : m_thing(thing) {
        m_superInfo = thing->TestThis();
    }
    TheOtherThing(std::shared_ptr<IAmAThing> thing, string sInfo)
        : m_thing(thing)
        , m_superInfo(sInfo) {
        m_superInfo = thing->TestThis() + ", '" + sInfo + "'";
    }
    string TheOtherTest() {
        return "TheOtherThing(" + m_superInfo + ")";
    }
};

class IHello
{
public:
    virtual void hello() const = 0;
    virtual ~IHello() = default;
};

class Hello : public IHello
{
public:
    virtual void hello() const {
        std::cout << "hello world!" << std::endl;
    }
};

class Privet : public IHello
{
public:
    virtual void hello() const {
        std::cout << "Privet world!" << std::endl;
    }
};

ioc::Container gContainer;
void demo2()
{
    //------Example #1----------------
    //Injector injector;
    ioc::Container injector;

    // Регистрируем IHello с классом Hello, т.о. каждый раз запрашивая IHell получаем объект Hello.
    injector.registerInstance<IHello, Hello>();
    auto helloInstance = injector.resolve<IHello>();
    helloInstance->hello();
    injector.registerInstance<IHello, Privet>();

    //Здесь, после регистрации получим объект Privet
    helloInstance = injector.resolve<IHello>();
    helloInstance->hello();

    //------Example #2----------------
    
    gContainer.registerInstance<IAmAThing, TheThing>();
    gContainer.registerInstance<IAmTheOtherThing, TheOtherThing, IAmAThing>();

    gContainer.resolve<IAmAThing>()->TestThis();
    gContainer.resolve<IAmTheOtherThing>()->TheOtherTest();

    //Опять запршиваем объект,после последней регистрации получим объект Privet
    helloInstance = injector.resolve<IHello>();
    helloInstance->hello();
}
