# Simple implementation of IoC/DI container

## What is DI and IoC
[IoC/DI container](https://martinfowler.com/articles/injection.html)

IoC/DI have three type of containers. 
    - contsructor injection
    - setter injection
    - interface injection

We are gonna implement constructor injection container.


```c++
IOCContainer container;

container.registerInstance<IFoo, Foo>();
auto foo = container.resolve<IFoo>();
foo->hello();
```

Registered `Foo` to `IFoo` instance and we next time call resolve `IFoo` it gives us registered to it `Foo` instance.

The main goal of this architechture is to avoid this type of cases:
```c++
class IConsole {
    virtual void log(string msg) = 0;
}
class Console : IConsole {
    Console(IWriter*, IPrinter*) {};
    void log(string msg) override {}
}


IConsole* c = new Console(new Writer(), new Printer(new Foo()))
```

Using this architechture we just need resolve `Console`
```c++
IOCContainer container;

// configure
container.registerInstance<IWriter, Writer>();
container.registerInstance<IFoo, Foo>();

container.registerFactory<IPrinter, Printer, IFoo>();
container.registerFactory<IConsole, Console, IWriter, IPrinter>();

// resolve console
IConsole* c = container.resolve<IConsole>();
c->log("di");
```

# Problems
- [ ] If `GetObject` is not find instance app crushing
- [ ] Cannot construct object with non inject arguments
- [ ] Circular dependencies (deadlock)
    ```c++
    class Foo : public IFoo {
        Foo(IBar* IBar) { ... }
    };
    class Bar : public IBar {
        Bar(IFoo* IFoo) { ... }
    };
    ```

    Cons of constructor injections method. We cannot create `Foo` w/o `Bar` and `Bar` w/o `Foo`. In `Container::registerInstance` we need to **prevent** this type of dependencies.

    But if we'd like to solve this need to use setter injections method.
    ```c++
    class Foo : public IFoo {
        void setBar(Bar* Bar) { ... }
    };
    class Bar : public Bar {
        void setFoo(IFoo* IFoo) { ... }
    };
    ```


https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
https://en.cppreference.com/w/cpp/types/type_index
https://en.cppreference.com/w/cpp/language/pack
https://en.cppreference.com/w/cpp/utility/functional/function

