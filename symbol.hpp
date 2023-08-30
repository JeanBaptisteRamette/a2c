#ifndef A2C_SYMBOL_HPP
#define A2C_SYMBOL_HPP


#include <cstdint>


namespace a2c
{
    namespace details
    {
        template<typename T>
        class BaseSymbol
        {
        public:
            explicit BaseSymbol(uintptr_t SymAddr) : SymAddress(SymAddr) {}

            T *Pointer() const {
                return reinterpret_cast<T *>(SymAddress);
            }

            explicit operator T *() const {
                return Pointer();
            }

            T *operator->() const {
                return Pointer();
            }

        private:
            uintptr_t SymAddress;
        };
    }

    template <typename T>
    struct Symbol : details::BaseSymbol<T>
    {
        using details::BaseSymbol<T>::BaseSymbol;
    };

    template <typename T, typename... Args>
    struct Symbol<T(Args...)> : details::BaseSymbol<T(Args...)>
    {
        using FunctionType = T(Args...);

        using details::BaseSymbol<FunctionType>::BaseSymbol;

        T operator()(Args... args)
        {
            return static_cast<FunctionType*>(this->Pointer())(args...);
        }
    };
}


#endif //A2C_SYMBOL_HPP
