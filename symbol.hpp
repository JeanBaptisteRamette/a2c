#ifndef A2C_SYMBOL_HPP
#define A2C_SYMBOL_HPP


#include <cstdint>
#include "mindef.hpp"


namespace a2c
{
    namespace details
    {
        template<typename T>
        class BaseSymbol
        {
        public:
            BaseSymbol(UPTR SymAddr) : SymAddress(SymAddr) {}

            [[nodiscard]] UPTR Address() const
            {
                return SymAddress;
            }

            [[nodiscard]] T* Pointer() const
            {
                return reinterpret_cast<T*>(SymAddress);
            }

            [[nodiscard]] explicit operator T *() const
            {
                return Pointer();
            }

            [[nodiscard]] T* operator -> () const
            {
                return Pointer();
            }

        private:
            UPTR SymAddress;
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

        T operator()(Args... args) const
        {
            return static_cast<FunctionType*>(this->Pointer())(args...);
        }
    };
}


#endif //A2C_SYMBOL_HPP
