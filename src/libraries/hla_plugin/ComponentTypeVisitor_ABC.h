// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ComponentTypeVisitor_ABC_h
#define plugins_hla_ComponentTypeVisitor_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ComponentTypeVisitor_ABC
    @brief  Component type visitor definition
*/
// Created: SLI 2011-09-29
// =============================================================================
class ComponentTypeVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ComponentTypeVisitor_ABC() {}
    virtual ~ComponentTypeVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyEquipment( unsigned int typeIdentifier, const std::string& typeName, unsigned int number ) = 0;
    //@}
};

}
}

#endif // plugins_hla_ComponentTypeVisitor_ABC_h
