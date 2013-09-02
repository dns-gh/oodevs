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
    class WorldLocation;
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
    typedef std::vector< char > T_UniqueId;
    enum GeometryType
    {
        eGeometryType_Point,
        eGeometryType_Line,
        eGeometryType_Polygon,
    };
    struct PropagationData
    {
        PropagationData( double lat, double lon, double concent ) : latitude(lat), longitude(lon), concentration(concent){}
        double latitude;
        double longitude;
        double concentration;
    };
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
    virtual void EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int available,
                    unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages ) = 0;
    virtual void UniqueIdChanged( const std::string& identifier, const T_UniqueId& uniqueId ) = 0;
    virtual void CallsignChanged( const std::string& identifier, const std::string& callsign ) = 0;
    virtual void EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& embeddedUnits ) = 0;
    virtual void GeometryChanged( const std::string& identifier, const std::vector< rpr::WorldLocation >& perimeter, GeometryType type ) = 0;
    virtual void ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId ) = 0;
    virtual void SubAgregatesChanged( const std::string& rtiIdentifier, const T_EntityIDs& children ) = 0;
    virtual void SubEntitiesChanged( const std::string& rtiIdentifier, const T_EntityIDs& children ) = 0;
    virtual void PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
            int col, int lig, double xll, double yll, double dx, double dy ) = 0;
    //@}
};
}
}
#endif // plugins_hla_ObjectListener_ABC_h
