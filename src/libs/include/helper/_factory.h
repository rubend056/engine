/**
 * @file  _factory.h
 * @author RubenD (rubendariopm14@gmail.com)
 * @brief Defines a factory implementation
 * @version 0.1
 * 
 */
#ifndef _factory_h
#define _factory_h

// #define FACTORY_BASE_CLASS Test

#ifndef FACTORY_KEY_TYPE
#define FACTORY_KEY_TYPE uint32_t
#endif

#ifndef FACTORY_BASE_CLASS
#error "No FACTORY_BASE_CLASS, make sure to define it as the base class for the factory"
#endif

#include <memory>

class FACTORY_BASE_CLASS;

/**
 * Variantiator is kind of like a template for on-demand creation of a class
 * 
 * _VariantinatorBase allows _VariantFactory to work will all of them as a common class, and
 * _Variantinator is an actual template that is created for every type (bit of code that actually knows what it is exactly that has to be made)
 */
class _VariantinatorBase {
   public:
	_VariantinatorBase() {}
	virtual ~_VariantinatorBase() {}
	virtual std::unique_ptr<FACTORY_BASE_CLASS> Create() = 0;
};

template <class T>
class _Variantinator : public _VariantinatorBase {
   public:
	_Variantinator() {}
	virtual ~_Variantinator() {}
	virtual std::unique_ptr<FACTORY_BASE_CLASS> Create() { return std::make_unique<T>(); }
};

/**
 * _VariantFactory handles all  _VariantinatorBase and creates based on 
 */
class _VariantFactory {
   public:
	_VariantFactory() {}

	/**
	 * Creates a pre-registered class based on its key
	 */
	std::shared_ptr<FACTORY_BASE_CLASS> create(FACTORY_KEY_TYPE type) {
		for (auto& v : kv) {
			if (v.first == type)
				return std::move(v.second->Create());
		}
		return nullptr;
	}

   private:
	// Key to Variant
	std::map<FACTORY_KEY_TYPE, std::unique_ptr<_VariantinatorBase>> kv;

	// Actual function
	void Register(FACTORY_KEY_TYPE k, std::unique_ptr<_VariantinatorBase>&& creator) {
		kv[k] = std::move(creator);
	}

   protected:
	// Template function for registration
	template <typename T>
	void Register(FACTORY_KEY_TYPE k) {
		Register(k, std::make_unique<_Variantinator<T>>());
	}
};

#endif	//  Factory_h