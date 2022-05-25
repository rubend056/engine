/**
 * @file  _factory.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines a factory implementation
 * @version 0.1
 * 
 */
#ifndef  _factory_h
#define  _factory_h

// #define FACTORY_BASE_CLASS Test

#ifndef FACTORY_KEY_TYPE
  #define FACTORY_KEY_TYPE uint32_t
#endif

#ifndef FACTORY_BASE_CLASS

	#error "No FACTORY_BASE_CLASS, make sure to define it as the base class for the factory"
#endif

#include <map>
#include <memory>

class FACTORY_BASE_CLASS;

class _VariantinatorBase {
  public:
    _VariantinatorBase() {}
    virtual ~_VariantinatorBase() {}
    virtual std::unique_ptr<FACTORY_BASE_CLASS> Create() = 0;
};

template< class T >
class _Variantinator : public _VariantinatorBase {
  public:
    _Variantinator() {}
    virtual ~_Variantinator() {}
    virtual std::unique_ptr<FACTORY_BASE_CLASS> Create() { return std::make_unique<T>(); }
};

class _VariantFactory
{
  public:
    _VariantFactory()
    {
         // If you want, you can do all your Register() calls in here, and even
         // make the Register() function private.
    }

    std::shared_ptr<FACTORY_BASE_CLASS> create( FACTORY_KEY_TYPE type )
    {
        auto it = int_switchToVariant.find( type );
        if( it == int_switchToVariant.end() ) return nullptr;
        return std::move(it->second->Create());
    }
    
  protected:
    template< FACTORY_KEY_TYPE type, typename T >
    void Register()
    {
        Register( type, std::make_unique<_Variantinator<T>>() );
    }

  
    void Register( FACTORY_KEY_TYPE type, std::unique_ptr<_VariantinatorBase>&& creator )
    {
        int_switchToVariant[type] = std::move(creator);
    }
  
    std::map<FACTORY_KEY_TYPE, std::unique_ptr<_VariantinatorBase> > int_switchToVariant;
};


#endif //  Factory_h