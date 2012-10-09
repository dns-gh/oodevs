// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ObjectListener_ABC_h
#define plugins_hla_ObjectListener_ABC_h

#include <rpr/ForceIdentifier.h>
#include <string>
#include <vector>
#include <set>

namespace rpr
{
    class EntityType;
    class PerimeterPoint;
}

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  ObjectListener_ABC
    @brief  ObjectListener_ABC
*/
// Created: AHC 2012-02-27
// =============================================================================
class ObjectListener_ABC
{
public:
    //! @name Types
    //@{
    typedef std::set< std::string > T_EntityIDs;
    //@}

    //! @name Constructors/Destructor
    //@{
    virtual ~ObjectListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Moved( const std::string& identifier, double latitude, double longitude ) = 0;
    virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side ) = 0;
    virtual void NameChanged( const std::string& identifier, const std::string& name ) = 0;
    virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type ) = 0;
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number ) = 0;
    virtual void UniqueIdChanged( const std::string& identifier, const std::string& uniqueId ) = 0;
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign ) = 0;
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< std::string >& embeddedUnits ) = 0;
    virtual void PerimeterChanged( const std::string& identifier, const std::vector< rpr::PerimeterPoint >& perimeter ) = 0;
    virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId ) = 0;
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const T_EntityIDs& children ) = 0;
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const T_EntityIDs& children ) = 0;

    //@}
};
}
}
#endif // plugins_hla_ObjectListener_ABC_h
