#include <memory>
#include <iostream>

using namespace std;

class IFoo;
class IWriter;
class IPrinter;


class IWriter {
public:
    IWriter() = default;
    virtual string write() = 0;
};
class Writer : public IWriter{
public:
    Writer() = default;
    Writer(Writer& ) {
        std::cout << "COPY\n";
    }
    virtual string write() {
        return "write()";
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
        return "foo()";
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
    Printer(Printer& ) {
        std::cout << "COPY\n";
    }

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
    Console(Console& ) {
        std::cout << "COPY\n";
    }
    virtual string log() {
        return "console(" + w->write() + ", " + p->print() + ")";
    }
};


class HandWriter : public IWriter {
    shared_ptr<IFoo> f;
public:
    HandWriter(shared_ptr<IFoo> f) : f(f) {}
    virtual string write() {
        return "handwrite(" + f->foo() + ")";
    }
};


