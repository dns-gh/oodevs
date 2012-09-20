// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CapacityFactory_ABC_h_
#define __CapacityFactory_ABC_h_

#include <boost/noncopyable.hpp>

class ObjectPrototype;
class MIL_Object_ABC;

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  CapacityFactory
    @brief  Capacity factory
*/
// Created: LGY 2012-09-19
// =============================================================================
class CapacityFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CapacityFactory_ABC() {}
    virtual ~CapacityFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Create( ObjectPrototype& protoype, const std::string& capacity, xml::xistream& xis ) const = 0;
    virtual void Update( MIL_Object_ABC& object, const std::string& capacity, xml::xistream& xis ) const = 0;
    virtual void FinalizeCreate( ObjectPrototype& object ) const = 0;
    //@}
};

#endif // __CapacityFactory_ABC_h_