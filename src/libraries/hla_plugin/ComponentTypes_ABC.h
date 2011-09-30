// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ComponentTypes_ABC_h
#define plugins_hla_ComponentTypes_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
    class ComponentTypeVisitor_ABC;

// =============================================================================
/** @class  ComponentTypes_ABC
    @brief  Component types definition
*/
// Created: SLI 2011-09-29
// =============================================================================
class ComponentTypes_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ComponentTypes_ABC() {}
    virtual ~ComponentTypes_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Apply( unsigned int agentTypeIdentifier, ComponentTypeVisitor_ABC& visitor ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_ComponentTypes_ABC_h
