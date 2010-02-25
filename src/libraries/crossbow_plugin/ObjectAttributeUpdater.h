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
    class MsgObjectAttributes;
    class MsgObjectAttributeConstruction;
    class MsgObjectAttributeObstacle;
    class MsgObjectAttributeMine;
    class MsgObjectAttributeActivityTime;
    class MsgObjectAttributeBypass;
    class MsgObjectAttributeLogistic;
    class MsgObjectAttributeNBC;
    class MsgObjectAttributeCrossingSite;
    class MsgObjectAttributeSupplyRoute;
    class MsgObjectAttributeToxicCloud;
    class MsgObjectAttributeFire;
    class MsgObjectAttributeMedicalTreatment;
    class MsgObjectAttributeInteractionHeight;
    class MsgObjectAttributeNBCType;
    class MsgObjectAttributeStock;
}

namespace plugins
{
namespace crossbow
{
class Database_ABC;
class InsertQueryBuilder;

class ObjectAttributeUpdater{
public:
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributes& msg );

private:
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeConstruction& construction );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeObstacle&  obstacle );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeMine&  mine );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeActivityTime&  activity_time );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeBypass&  bypass );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeLogistic&  logistic );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeNBC&  nbc );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeCrossingSite&  crossing_site );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeSupplyRoute&  supply_route );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeToxicCloud&  toxic_cloud );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeFire&  fire );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeMedicalTreatment&  medical_treatment );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeInteractionHeight&  interaction_height );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeNBCType&  nbc_agent );
    static void UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeStock & stock );
};

}
}
#endif // __crossbow_ObjectAttributeUpdater_h_
