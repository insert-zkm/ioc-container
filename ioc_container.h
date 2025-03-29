#ifndef IOC_CONTAINER
#define IOC_CONTAINER

#include <functional>
#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>

using namespace std;
namespace ioc {

    class Container
    {
    public:
        class FactoryRoot
        {
        public:
            virtual ~FactoryRoot() {}
        };

        //Получить экземпляр объекта
        template<typename T>
        class CFactory : public FactoryRoot
        {
            std::function<std::shared_ptr<T>()> m_functor;

        public:
            ~CFactory() {}

            CFactory(std::function<std::shared_ptr<T>()> functor)
                : m_functor(functor)
            {}

            std::shared_ptr<T> GetObject() {
                return m_functor();
            }
        };

        template<typename T>
        std::shared_ptr<T> GetObject() {
            auto factoryBase = m_factories[typeid(T)];
            auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
            return factory->GetObject();
        }

        //Регистрация экземпляров

        //Самая простая реализация - зарегистрировать функтор
        template<typename TInterface, typename... TS>
        void RegisterFunctor(
            std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS>... ts)> functor
        ) {
            m_factories[typeid(TInterface)] = std::make_shared<CFactory<TInterface>>(
                    [ = ] { return functor(GetObject<TS>()...); });
        }

        //Регистрация одного экземпляра объекта
        template<typename TInterface>
        void RegisterInstance(std::shared_ptr<TInterface> t) {
            m_factories[typeid(TInterface)] = std::make_shared<CFactory<TInterface>>(
                    [ = ] { return t; });
        }

        //Подаем указатель на функцию
        template<typename TInterface, typename... TS>
        void RegisterFunctor(std::shared_ptr<TInterface> (*functor)(std::shared_ptr<TS>... ts)) {
            RegisterFunctor(
                std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS>... ts)>(functor));
        }

        //Фабрика, которая будет вызывать конструктор, для каждого экземпляра
        template<typename TInterface, typename TConcrete, typename... TArguments>
        void RegisterFactory() {
            RegisterFunctor(
                std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments>... ts)>(
            [](std::shared_ptr<TArguments>... arguments) -> std::shared_ptr<TInterface> {
                return std::make_shared<TConcrete>(
                    std::forward<std::shared_ptr<TArguments>>(arguments)...);
            }));
        }

        //Фабрика, которая будет возвращать один экземпляр
        template<typename TInterface, typename TConcrete, typename... TArguments>
        void RegisterInstance() {
            RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
        }

    protected:
        std::map<std::type_index, std::shared_ptr<FactoryRoot>> m_factories;
    };
}
#endif // IOC_CONTAINER
