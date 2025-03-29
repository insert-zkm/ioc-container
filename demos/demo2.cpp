#include <memory>
#include <iostream>
#include <string>

#include "ioc_container.h"

class IAmAThing
{
public:
    virtual ~IAmAThing() {}
    virtual void TestThis() = 0;
};

class IAmTheOtherThing
{
public:
    virtual ~IAmTheOtherThing() {}
    virtual void TheOtherTest() = 0;
};

class TheThing : public IAmAThing
{
public:
    TheThing() {}
    void TestThis() {
        std::cout << "A Thing" << std::endl;
    }
};

class TheOtherThing : public IAmTheOtherThing
{
    std::shared_ptr<IAmAThing> m_thing;
    string m_superInfo;

public:
    TheOtherThing(std::shared_ptr<IAmAThing> thing)
        : m_thing(thing) {
        m_superInfo = "From TheOtherThing";
    }
    TheOtherThing(std::shared_ptr<IAmAThing> thing, string sInfo)
        : m_thing(thing)
        , m_superInfo(sInfo) {
        m_superInfo = sInfo;
    }
    void TheOtherTest() {
        std::cout << m_superInfo << std::endl;
        m_thing->TestThis();
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
    injector.RegisterInstance<IHello, Hello>();
    auto helloInstance = injector.GetObject<IHello>();
    helloInstance->hello();
    injector.RegisterInstance<IHello, Privet>();

    //Здесь, после регистрации получим объект Privet
    helloInstance = injector.GetObject<IHello>();
    helloInstance->hello();

    //------Example #2----------------

    gContainer.RegisterInstance<IAmAThing, TheThing>();
    gContainer.RegisterFactory<IAmTheOtherThing, TheOtherThing, IAmAThing>();

    gContainer.GetObject<IAmAThing>()->TestThis();
    gContainer.GetObject<IAmTheOtherThing>()->TheOtherTest();

    //Опять запршиваем объект,после последней регистрации получим объект Privet
    helloInstance = injector.GetObject<IHello>();
    helloInstance->hello();
}
