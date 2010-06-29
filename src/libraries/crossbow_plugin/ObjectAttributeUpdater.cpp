// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "protocol/protocol.h"
#include "ObjectAttributeUpdater.h"
#include "QueryBuilder.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"

using namespace plugins;
using namespace plugins::crossbow;


void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributes& msg ){
    if( msg.has_activity_time() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.activity_time() );
    if( msg.has_bypass() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.bypass() );
    if( msg.has_construction() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.construction() );
    if( msg.has_crossing_site() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.crossing_site() );
    if( msg.has_fire() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.fire() );
    if( msg.has_interaction_height() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.interaction_height() );
    if( msg.has_logistic() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.logistic() );
    if( msg.has_medical_treatment() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.medical_treatment() );
    if( msg.has_mine() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.mine() );
    if( msg.has_nbc() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.nbc() );
    if( msg.has_nbc_agent() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.nbc_agent() );
    if( msg.has_obstacle() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.obstacle() );
    if( msg.has_stock() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.stock() );
    if( msg.has_supply_route() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.supply_route() );
    if( msg.has_toxic_cloud() )
        ObjectAttributeUpdater::UpdateObjectAttribute( db, objectId, msg.toxic_cloud() );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeConstruction& construction ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Construction" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    if( construction.has_density() )
        builder.SetField( "density", construction.density() ); //real
    if( construction.has_percentage() )
        builder.SetField( "percentage", construction.percentage() ); //int
    if( construction.has_dotation_type() )
        builder.SetField( "dotation_type", construction.dotation_type() ); //int
    if( construction.has_dotation_nbr() )
        builder.SetField( "dotation_nbr", construction.dotation_nbr() ); //int

    db.Execute( builder );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeObstacle& obstacle ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Obstacle" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "activated", obstacle.activated() ); // bool
    // JSR
    builder.SetField( "type", obstacle.type() == Common::ObstacleType_DemolitionTargetType_preliminary ? "preliminary" : "reserved" );

    db.Execute( builder );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeMine& mine ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Mine" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    if( mine.has_density() )
        builder.SetField( "density", mine.density() );
    if( mine.has_percentage() )
        builder.SetField( "percentage", mine.percentage() );
    if( mine.has_dotation_type() )
        builder.SetField( "dotation_type", mine.dotation_type() );
    if( mine.has_dotation_nbr() )
        builder.SetField( "dotation_nbr", mine.dotation_nbr() );

    db.Execute( builder );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeActivityTime& activity_time ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Activity_Time" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "activity_time", activity_time.value() ); // int

    db.Execute( builder );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeBypass& bypass ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Bypass" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    if( bypass.has_percentage() )
        builder.SetField( "percentage", bypass.percentage() ); //int

    db.Execute( builder );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeLogistic& logistic ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Logistic" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "tc2", logistic.tc2() ); // automat id

    db.Execute( builder );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeNBC& nbc ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_NBC" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "danger_level", nbc.danger_level() ); // int
    // nbc.nbc_agents // list object: TODO

    db.Execute( builder );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeCrossingSite& crossing_site ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Crossing_Site" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );

    builder.SetField( "banks_require_fitting", crossing_site.banks_require_fitting() ); //boolean
    builder.SetField( "depth", crossing_site.depth() ); //int
    builder.SetField( "flow_rate", crossing_site.flow_rate() ); //int
    builder.SetField( "width", crossing_site.width() ); //int

    db.Execute( builder );
}
void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeSupplyRoute& supply_route ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_SupplyRoute" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "equipped", supply_route.equipped() ); // boolean
    builder.SetField( "flow_rate", supply_route.flow_rate() ); // int
    builder.SetField( "length", supply_route.length() ); // int
    builder.SetField( "max_weight", supply_route.max_weight() ); // int
    builder.SetField( "width", supply_route.width() ); // int

    db.Execute( builder );
}
void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeToxicCloud& toxic_cloud ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Toxic_Cloud" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    //toxic_cloud.quantities // list: todo
    db.Execute( builder );
}
void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeFire& fire ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Fire" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "class_id", fire.class_id() ); // int
    builder.SetField( "heat", fire.heat() ); // int

    db.Execute( builder );
}
void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeMedicalTreatment& medical_treatment ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Medical_Treatment" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "doctors", medical_treatment.doctors() ); // int
    builder.SetField( "beds", medical_treatment.beds() ); // int
    builder.SetField( "available_doctors", medical_treatment.available_doctors() ); // int
    builder.SetField( "available_beds", medical_treatment.available_beds() ); // int

    db.Execute( builder );
}
void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeInteractionHeight& interaction_height ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Interaction_Height" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "height", interaction_height.height() ); //real

    db.Execute( builder );
}
void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeNBCType& nbc_agent ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_NBC_Type" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    builder.SetField( "agent_id", nbc_agent.agent_id() ); // int
    builder.SetField( "concentration", nbc_agent.concentration() ); // int
    builder.SetField( "source_life_duration", nbc_agent.source_life_duration() ); // int

    db.Execute( builder );
}

void ObjectAttributeUpdater::UpdateObjectAttribute( Database_ABC& db, long objectId, const Common::MsgObjectAttributeStock& stock ){
    InsertQueryBuilder builder( db.GetTableName( "TacticalObject_Attribute_Stock" ) );

    builder.SetId( "id" );
    builder.SetField( "object_id", objectId );
    //todo: list : stock.resources
    db.Execute( builder );
}
