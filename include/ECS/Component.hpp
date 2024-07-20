#pragma once

/**
 * @file Component.hpp
 * 
 * This file defines the Component base class, as well
 * as the many macros used to define Component subclasses.
 * 
 * @note This, and most base ECS classes, were adapted from
 * [Austin Morlan's amazing ECS writeup here.](https://austinmorlan.com/posts/entity_component_system/)
 * 
 * @author Tim Bishop
*/

#include <cstdint>
#include <bitset>

#include <any>
#include <map>
#include <functional>

#include <boost/preprocessor.hpp>

// z, data, elem
#define PRINT_TO_SETTER(z, data, elem) BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, elem),\
void BOOST_PP_CAT(__set_, BOOST_PP_TUPLE_ELEM(3, elem))(Property p)\
{BOOST_PP_TUPLE_ELEM(3, elem) = std::any_cast<BOOST_PP_TUPLE_ELEM(2, elem)>(p); }, \
)

#define PRINT_TO_COPYSTATEMENT(z, data, elem) BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, elem),\
BOOST_PP_TUPLE_ELEM(3, elem) = cpy.BOOST_PP_TUPLE_ELEM(3, elem);, \
)

#define PRINT_TO_PROPERTY_PT1(z, data, elem) BOOST_PP_IF(  BOOST_PP_TUPLE_ELEM( 0 , elem ),\
BOOST_PP_IF(  BOOST_PP_EQUAL(  BOOST_PP_TUPLE_SIZE( elem ), 5  ),\
(BOOST_PP_TUPLE_ELEM(1, elem): BOOST_PP_TUPLE_ELEM(2, elem) BOOST_PP_TUPLE_ELEM(3, elem) = BOOST_PP_TUPLE_ELEM(4, elem);),\
(BOOST_PP_TUPLE_ELEM(1, elem): BOOST_PP_TUPLE_ELEM(2, elem) BOOST_PP_TUPLE_ELEM(3, elem);)),\
BOOST_PP_TUPLE_ELEM(1, elem)\
)

#define PRINT_TO_PROPERTY(z, data, elem) BOOST_PP_EXPAND(BOOST_PP_TUPLE_REM()PRINT_TO_PROPERTY_PT1(z, data, elem))

#define LAMBDA(elem) _setters[BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(3, elem))] = [=](Property p){\
BOOST_PP_CAT(__set_, BOOST_PP_TUPLE_ELEM(3, elem))(p); };

#define PRINT_TO_LAMBDA(z, data, elem) BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(0, elem),\
LAMBDA(elem), \
)

#define ROCKET_PROPERTY(qualifier, type, name) (1, qualifier, type, name)
#define ROCKET_PROPERTY_DEFVAL(qualifier, type, name, defval) (1, qualifier, type, name, defval)

// Random things in macro expansion to make tuple size > 3... screw off boost_pp :(
#define ROCKET_RAW(...) (0, (__VA_ARGS__), 1, 2)

#define ROCKET_COMPONENT(cname, ...) class cname : public Component {\
public: \
static std::string name() { return #cname; }\
BOOST_PP_SEQ_FOR_EACH(PRINT_TO_SETTER, _, BOOST_PP_VARIADIC_SEQ_TO_SEQ(__VA_ARGS__)) \
BOOST_PP_SEQ_FOR_EACH(PRINT_TO_PROPERTY, _, BOOST_PP_VARIADIC_SEQ_TO_SEQ(__VA_ARGS__)) \
\
public: cname() {\
    BOOST_PP_SEQ_FOR_EACH(PRINT_TO_LAMBDA, _, BOOST_PP_VARIADIC_SEQ_TO_SEQ(__VA_ARGS__)) \
}\
\
public: cname(const cname & cpy) {\
    BOOST_PP_SEQ_FOR_EACH(PRINT_TO_COPYSTATEMENT, _, BOOST_PP_VARIADIC_SEQ_TO_SEQ(__VA_ARGS__)) \
    BOOST_PP_SEQ_FOR_EACH(PRINT_TO_LAMBDA, _, BOOST_PP_VARIADIC_SEQ_TO_SEQ(__VA_ARGS__)) \
} \
\
public: cname & operator=(const cname & cpy) {\
    BOOST_PP_SEQ_FOR_EACH(PRINT_TO_COPYSTATEMENT, _, BOOST_PP_VARIADIC_SEQ_TO_SEQ(__VA_ARGS__)) \
    BOOST_PP_SEQ_FOR_EACH(PRINT_TO_LAMBDA, _, BOOST_PP_VARIADIC_SEQ_TO_SEQ(__VA_ARGS__)) \
    return *this;\
}\
}

using Property = std::any;

using ComponentType = std::uint16_t;


/** A constant unsigned int representing the maximum number of components allowed. */
const ComponentType MAX_COMPONENTS = 256;

using Signature = std::bitset<MAX_COMPONENTS>;

/**
 * @class Component
 * 
 * A quick base class, just so that any components
 * created all implement certain Constructors and have
 * a public "isNull" boolean.
*/
class Component
{
private:
    bool mIsNull;

public:
    /**
     * A map of setter functions for any RocketProperties
     * inside a subclass.
     * 
     * @note This map is only public to work with LoadScene().
     * 
     * @todo Make this class have a friend function of LoadScene()
     * and make _setters protected.
    */
    std::map<std::string, std::function<void(Property)>> _setters;

    /**
     * Default constructor
     * 
     * @param null Is the created component null?
     * 
     * @todo Remove all instances of isNull and its properties -
     * they serve no purpose.
    */
    Component(bool null = false) {mIsNull = null;}
    bool isNull() {return mIsNull;}

    /**
     * A virtual destructor function, not defined explicitly
     * as a destructor so we have more control over when a
     * Component (or subclass of it) should be destroyed.
    */
    virtual void DestroyComponent() {}
};
