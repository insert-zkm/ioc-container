#include <memory>

#include "ioc_container.h"

class IWriter {
public:
    IWriter() = default;
    virtual string write() = 0;
};
class Writer : public IWriter{
public:
    Writer() = default;
    virtual string write() {
        return "write";
    }
};

class IFoo {
public:
    virtual string foo() = 0;
};
class Foo : public IFoo {
public:
    Foo() = default;
    Foo(Foo& ) {
        std::cout << "COPY\n";
    }
    virtual string foo() {
        return "foo";
    }
};

class IPrinter {
public:
    virtual string print() = 0;
};
class Printer : public IPrinter {
public:
    std::shared_ptr<IFoo> foo;
    Printer(std::shared_ptr<IFoo> foo) : foo(foo) {};

    virtual string print() {
        return "print(" + foo->foo() + ")";
    }
};

class IConsole {
public:
    virtual string log() = 0;
};
class Console : public IConsole {
public:
    std::shared_ptr<IWriter> w;
    std::shared_ptr<IPrinter> p;
    Console(std::shared_ptr<IWriter> w, std::shared_ptr<IPrinter> p) : w(w), p(p) {};
    virtual string log() {
        return "console(" + w->write() + ", " + p->print() + ")";
    }
};


void demo1() { // IConsole* c = new Console(new Writer(), new Printer(new Foo()))
    ioc::Container container;

    // configure
    container.RegisterInstance<IWriter, Writer>();
    container.RegisterInstance<IFoo, Foo>();
    container.RegisterInstance<IPrinter, Printer, IFoo>();
    container.RegisterInstance<IConsole, Console, IWriter, IPrinter>();

    // re/use
    std::shared_ptr<IConsole> c = container.GetObject<IConsole>();
    std::cout << c->log();
}
