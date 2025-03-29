#include <functional>
#include <map>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <cassert>

namespace ioc_copy {

class Container {
protected:
    class IFactory;
    typedef std::map<std::type_index, std::shared_ptr<IFactory>> FactoryMap;

    class IFactory {
    public:
        virtual ~IFactory() {}
    };

    template<typename T>
    class CFactory : public IFactory {
    protected:
        std::function<std::shared_ptr<T>()> _func;
    public:
        CFactory(std::function<std::shared_ptr<T>()> func) : _func(func) {}
        ~CFactory() {}

        std::shared_ptr<T> getObject() const {
            return _func();
        }
    };

    template<typename Base, typename Derived>
    inline static bool isInstanceOf() {
        return std::is_base_of<Base, Derived>::value;
    }

    template<typename Base>
    void registerFactory(
        std::function<std::shared_ptr<Base>()> factory_func
    ) {
        this->_factories[typeid(Base)] = std::make_shared<CFactory<Base>>(factory_func);
    }

public:
    template<typename Base>
    std::shared_ptr<Base> resolve() const {
        std::type_index key = typeid(Base);

        FactoryMap::const_iterator factory = this->_factories.find(key);
        if(factory == this->_factories.cend()) {
            return nullptr;
        }

        std::shared_ptr<CFactory<Base>> factory_func = std::static_pointer_cast<CFactory<Base>>(factory->second);

        return factory_func->getObject();
    }



    template<typename Base, typename Derived, typename ...TArgs>
    void registerInstance() {
        assert(
            (this->isInstanceOf<Base, Derived>())
        );

        std::function<std::shared_ptr<Base>()> factory_func = [this] () {
            return std::make_shared<Derived>(this->resolve<TArgs>()...);
        };

        registerFactory<Base>(factory_func);
    }

protected:
    FactoryMap _factories;
};
}
/*
map {
    {
        IFoo,
        (): IFoo* {
            return new Foo();
        }
    },
    {
        IWriter,
        (): IWriter* {
            return new Writer();
        }
    },
    {
        IPrinter,
        (): IPrinter* {
            return new Printer(resolve<IFoo>);
        }
    },
    {
        IConsole,
        (): IConsole* {
            return new Console(resolve<IWriter>, resolve<IPrinter>)
        }
    }
}
*/



