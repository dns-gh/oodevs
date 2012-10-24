// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASAGroup
//
// *****************************************************************************

#ifndef plugins_hla_EntityIdResolver_ABC_h
#define plugins_hla_EntityIdResolver_ABC_h

#include <string>

namespace rpr
{
    class EntityIdentifier;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  EntityIdentifierResolver_ABC
    @brief  EntityIdentifierResolver_ABC
*/
// Created: AHC 2012-04-22
// =============================================================================
class EntityIdentifierResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityIdentifierResolver_ABC() {}
    virtual ~EntityIdentifierResolver_ABC() {}

    //! @name Operations
    //@{
    virtual void Create( const std::string& name, rpr::EntityIdentifier& id ) = 0; // Create and register an ID
    virtual void Register( const rpr::EntityIdentifier& identifier, const std::string& name ) = 0;
    virtual void Unregister( const rpr::EntityIdentifier& identifier ) = 0;
    virtual void Unregister( const std::string& name ) = 0;
    virtual const std::string& Resolve( const rpr::EntityIdentifier& identifier ) const = 0;
    virtual const rpr::EntityIdentifier& Resolve( const std::string& name ) const = 0;
    //@}
};
}
}

#endif // plugins_hla_EntityIdResolver_ABC_h
