#include "demo1.hpp"
#include "demo2.hpp"

void test1() {
    ioc::Container container;

    // configure
    container.registerInstance<IWriter, Writer>();
    container.registerInstance<IFoo, Foo>();
    container.registerInstance<IPrinter, Printer, IFoo>();
    container.registerInstance<IConsole, Console, IWriter, IPrinter>();

    // re/use
    std::shared_ptr<IConsole> c = container.resolve<IConsole>();    // IConsole* c = new Console(new Writer(), new Printer(new Foo()))
    std::shared_ptr<IPrinter> p = container.resolve<IPrinter>();    // IPrinter * p = new Printer(new Foo())
    std::shared_ptr<IFoo> f = container.resolve<IFoo>();            // IFoo* f = new Foo()
    std::shared_ptr<IWriter> w = container.resolve<IWriter>();      // IWriter* w = new Writer()


    assert(( f->foo() == "foo" ));
    assert(( w->write() == "write" ));
    assert(( p->print() == "print(foo)" ));
    assert(( c->log() == "console(write, print(foo))" ));

    container.registerInstance<IWriter, HandWriter, IFoo>();
    std::shared_ptr<IWriter> hw = container.resolve<IWriter>();     // IWriter* w = new HandWriter(new IFoo())

    assert(( hw->write() == "handwrite(foo)" ));
}

void test2() {
    ioc::Container container;
    container.registerInstance<IPrinter, Printer, IFoo>();
}

void test() {
//    test1();
    test2();
}

