// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ObjectListenerComposite_h
#define plugins_hla_ObjectListenerComposite_h

#include "ObjectListener_ABC.h"
#include <set>

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
class ObjectListenerComposite : public ObjectListener_ABC
{
public:
    ObjectListenerComposite();
    virtual ~ObjectListenerComposite();


    //! @name Operations
   //@{
   virtual void Moved( const std::string& identifier, double latitude, double longitude );
   virtual void SideChanged( const std::string& identifier, rpr::ForceIdentifier side );
   virtual void NameChanged( const std::string& identifier, const std::string& name );
   virtual void TypeChanged( const std::string& identifier, const rpr::EntityType& type );
   virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int available,
           unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages );
   virtual void UniqueIdChanged( const std::string& identifier, const T_UniqueId& uniqueId );
   virtual void CallsignChanged( const std::string& identifier, const std::string& callsign );
   virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& embeddedUnits );
   virtual void GeometryChanged( const std::string& identifier, const std::vector< rpr::WorldLocation >& perimeter, ObjectListener_ABC::GeometryType type );
   virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId );
   virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
   virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const ObjectListener_ABC::T_EntityIDs& children );
   virtual void PropagationChanged( const std::string& , const std::vector< PropagationData >&, int, int, double, double, double, double );

   virtual void Register( ObjectListener_ABC& listener );
   virtual void Unregister( ObjectListener_ABC& listener );
   //@}

private:
   typedef std::set<ObjectListener_ABC*> T_Listeners;

private:
   T_Listeners listeners_;
};

}
}

#endif // plugins_hla_ObjectListenerComposite_h
