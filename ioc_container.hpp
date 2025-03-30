#ifndef IOC_CONTAINER
#define IOC_CONTAINER

#include <functional>
#include <map>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <cassert>

namespace ioc {

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

    template<typename T>
    bool contains() const {
        return (this->_factories.find(typeid(T)) != this->_factories.cend());
    }

    template<typename ...TArgs>
    bool isArgsContain() const {
        const int size = sizeof...(TArgs);
        bool chk[size] = {contains<TArgs>()...};

        for(int i = 0; i < size; i++) {
            if (!chk[i]) {
                return false;
            }
        }

        return true;
    }
public:
    template<typename Base>
    std::shared_ptr<Base> resolve() const {
        std::type_index key = typeid(Base);

        FactoryMap::const_iterator factory = this->_factories.find(key);
        assert(( factory != this->_factories.cend() ));

        std::shared_ptr<CFactory<Base>> factory_func = std::static_pointer_cast<CFactory<Base>>(factory->second);

        return factory_func->getObject();
    }



    template<typename Base, typename Derived, typename ...Injections>
    void registerInstance() {
        assert(( this->isInstanceOf<Base, Derived>() && "Container::registerInstance<Base, Derived, ...Injections>() Derived is not instance of Base" ));

        assert(( this->isArgsContain<Injections...>() && "Container::registerInstance<Base, Derived, ...Injections>() Injections is not in Container" ));

        std::function<std::shared_ptr<Base>()> factory_func = [this] () {
            return std::make_shared<Derived>(this->resolve<Injections>()...);
        };

        registerFactory<Base>(factory_func);
    }

protected:
    FactoryMap _factories;
};
}

#endif // IOC_CONTAINER

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


