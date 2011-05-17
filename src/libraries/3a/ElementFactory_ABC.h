// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ElementFactory_ABC_h_
#define __ElementFactory_ABC_h_

#include <string>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

class Task;

// =============================================================================
/** @class  ElementFactory_ABC
    @brief  Element factory definition
*/
// Created: AGE 2008-08-04
// =============================================================================
class ElementFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ElementFactory_ABC() {}
    virtual ~ElementFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void CreateElement( const std::string& type, xml::xistream& xis, Task& result ) const = 0;
    //@}
};

#define TYPE_DISPATCH_HELPER( name, base )                                          \
struct base::name##Dispatcher                                                       \
{                                                                                   \
    name##Dispatcher( const base* that ): that_( that ) {}                          \
    template< typename T >                                                          \
    void operator()( const std::string& n, xml::xistream& xis, Task& result ) const \
    {                                                                               \
        that_->name< T >( n, xis, result );                                         \
    }                                                                               \
    const base* that_;                                                              \
};

#endif // __ElementFactory_ABC_h_
