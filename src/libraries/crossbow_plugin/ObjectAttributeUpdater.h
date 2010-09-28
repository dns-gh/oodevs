// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_ObjectAttributeUpdater_h_
#define __crossbow_ObjectAttributeUpdater_h_

namespace Common
{
    class ObjectAttributes;
    class ObjectAttributeConstruction;
    class ObjectAttributeObstacle;
    class ObjectAttributeMine;
    class ObjectAttributeActivityTime;
    class ObjectAttributeBypass;
    class ObjectAttributeLogistic;
    class ObjectAttributeNBC;
    class ObjectAttributeCrossingSite;
    class ObjectAttributeSupplyRoute;
    class ObjectAttributeToxicCloud;
    class ObjectAttributeFire;
    class ObjectAttributeMedicalTreatment;
    class ObjectAttributeInteractionHeight;
    class ObjectAttributeNBCType;
    class ObjectAttributeStock;
}

namespace plugins
{
namespace crossbow
{
class Database_ABC;
class InsertQueryBuilder;

class ObjectAttributeUpdater{
public:
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributes& msg );

private:
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeConstruction& construction );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeObstacle&  obstacle );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeMine&  mine );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeActivityTime&  activity_time );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeBypass&  bypass );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeLogistic&  logistic );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeNBC&  nbc );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeCrossingSite&  crossing_site );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeSupplyRoute&  supply_route );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeToxicCloud&  toxic_cloud );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeFire&  fire );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeMedicalTreatment&  medical_treatment );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeInteractionHeight&  interaction_height );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeNBCType&  nbc_agent );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::ObjectAttributeStock & stock );
};

}
}
#endif // __crossbow_ObjectAttributeUpdater_h_
