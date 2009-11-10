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

#include "game_asn/Simulation.h"

namespace plugins
{
namespace crossbow
{
class Database_ABC;
class InsertQueryBuilder;

class ObjectAttributeUpdater{
public:
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributes& msg );

private:
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeConstruction& construction );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeObstacle&  obstacle );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeMine&  mine );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeActivityTime&  activity_time );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeBypass&  bypass );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeLogistic&  logistic );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeNBC&  nbc );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeCrossingSite&  crossing_site );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeSupplyRoute&  supply_route );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeToxicCloud&  toxic_cloud );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeFire&  fire );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeMedicalTreatment&  medical_treatment );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeInteractionHeight&  interaction_height );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeNBCType&  nbc_agent );
	static void UpdateObjectAttribute( Database_ABC& db, long objectId, const ASN1T_ObjectAttributeStock & stock );
};

}
}
#endif // __crossbow_ObjectAttributeUpdater_h_
