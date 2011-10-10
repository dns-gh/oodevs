// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_RemoteAgentListener_ABC_h
#define plugins_hla_RemoteAgentListener_ABC_h

#include "rpr/ForceIdentifier.h"

namespace rpr
{
    class EntityType;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  RemoteAgentListener_ABC
    @brief  Remote agent listener definition
*/
// Created: SLI 2011-08-29
// =============================================================================
class RemoteAgentListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RemoteAgentListener_ABC() {}
    virtual ~RemoteAgentListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Created( const std::string& identifier ) = 0;
    virtual void Destroyed( const std::string& identifier ) = 0;
    virtual void Moved( const std::string& identifier, double latitude, double longitude ) = 0;
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side ) = 0;
    virtual void NameChanged( const std::string& identifier, const std::string& name ) = 0;
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type ) = 0;
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number ) = 0;
    virtual void UniqueIdChanged( const std::string& identifier, const std::string& uniqueId ) = 0;
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign ) = 0;
    //@}
};

}
}

#endif // plugins_hla_RemoteAgentListener_ABC_h
