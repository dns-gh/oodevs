// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "protocol/Protocol.h"
#include "ObjectAttributeUpdater.h"
#include "Workspace_ABC.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "WorkingSession_ABC.h"
#include <boost/noncopyable.hpp>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater constructor
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
ObjectAttributeUpdater::ObjectAttributeUpdater( Workspace_ABC& workspace, const WorkingSession_ABC& session, long objectId )
    : workspace_( workspace )
    , session_ ( session )
    , objectId_ ( objectId )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater destructor
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
ObjectAttributeUpdater::~ObjectAttributeUpdater()
{
    // NOTHING
}

#define CHECK_ATTRIBUTE_UPDATE( ATTR ) \
    if( msg.has_##ATTR##() ) \
        Update( msg.##ATTR##() )


void ObjectAttributeUpdater::Update( const sword::ObjectAttributes& msg )
{
    CHECK_ATTRIBUTE_UPDATE( activity_time   );
    CHECK_ATTRIBUTE_UPDATE( bypass          );
    CHECK_ATTRIBUTE_UPDATE( construction    );
    CHECK_ATTRIBUTE_UPDATE( crossing_site   );
    CHECK_ATTRIBUTE_UPDATE( fire            );
    CHECK_ATTRIBUTE_UPDATE( interaction_height );
    CHECK_ATTRIBUTE_UPDATE( logistic        );
    CHECK_ATTRIBUTE_UPDATE( medical_treatment );
    CHECK_ATTRIBUTE_UPDATE( mine            );
    CHECK_ATTRIBUTE_UPDATE( nbc             );
    CHECK_ATTRIBUTE_UPDATE( nbc_agent       );
    CHECK_ATTRIBUTE_UPDATE( obstacle        );
    CHECK_ATTRIBUTE_UPDATE( stock           );
    CHECK_ATTRIBUTE_UPDATE( supply_route    );
    CHECK_ATTRIBUTE_UPDATE( toxic_cloud     );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::OpenTable
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
Table_ABC* ObjectAttributeUpdater::OpenTable( const std::string& name )
{
    return workspace_.GetDatabase( "flat" ).OpenTable( name );
}

namespace 
{
    class RowManipulator : boost::noncopyable
    {
    public:
        RowManipulator( Table_ABC& table, const WorkingSession_ABC& session, int objectId )
            : table_ ( table )
            , updating_ ( true )
        {
            std::stringstream query;
            query << "object_id=" << objectId << " AND session_id=" << session.GetId();
            row_ = table_.Find( query.str() );
            if( !row_ )
            {
                row_ = &table_.CreateRow();
                row_->SetField( "object_id", FieldVariant( objectId ) );
                row_->SetField( "session_id", FieldVariant( session.GetId() ) );
                updating_ = false;
            }
        }
        
        ~RowManipulator()
        {
            if( updating_ )
                table_.UpdateRow( *row_ );
            else
                table_.InsertRow( *row_ );
        }

        void SetField( const std::string& field, const FieldVariant& value )
        {
            row_->SetField( field, value );
        }
        
    private:
        Table_ABC&  table_;
        Row_ABC*    row_;
        bool        updating_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeConstruction& construction )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Construction" ) );

    RowManipulator row( *table, session_, objectId_ );

    if( construction.density() )
        row.SetField( "density", FieldVariant( construction.density() ) ); //real
    if( construction.has_percentage() )
        row.SetField( "percentage", FieldVariant( construction.percentage() ) ); //int
    if( construction.has_resource() )
        row.SetField( "dotation_type", FieldVariant( static_cast< long >( construction.resource().id() ) ) ); //int
    if( construction.has_dotation() )
        row.SetField( "dotation_nbr", FieldVariant( construction.dotation() ) ); //int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeObstacle& obstacle )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Obstacle" ) );

    RowManipulator row( *table, session_, objectId_ );
    row.SetField( "activated", FieldVariant( obstacle.activated() ) ); // bool
    row.SetField( "type", FieldVariant( std::string( obstacle.type() == sword::ObstacleType_DemolitionTargetType_preliminary ? "preliminary" : "reserved" ) ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeMine& mine )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Mine" ) );

    RowManipulator row( *table, session_, objectId_ );
    if( mine.has_density() )
        row.SetField( "density", FieldVariant( mine.density() ) );
    if( mine.has_percentage() )
        row.SetField( "percentage", FieldVariant( mine.percentage() ) );
    if( mine.has_resource() )
        row.SetField( "dotation_type", FieldVariant( (long)mine.resource().id() ) );
    if( mine.has_dotation() )
        row.SetField( "dotation_nbr", FieldVariant( mine.dotation() ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeActivityTime& activity_time )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Activity_Time" ) );

    RowManipulator row( *table, session_, objectId_ );
    row.SetField( "activity_time", FieldVariant( activity_time.value() ) ); // int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeBypass& bypass )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Bypass" ) );

    RowManipulator row( *table, session_, objectId_ );
    if( bypass.has_percentage() )
        row.SetField( "percentage", FieldVariant( bypass.percentage() ) ); //int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeLogistic& logistic )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Logistic" ) );

    RowManipulator row( *table, session_, objectId_ );
    row.SetField( "tc2", FieldVariant( static_cast< long >( logistic.tc2().id() ) ) ); // automat id
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeNBC& nbc )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_NBC" ) );

    RowManipulator row( *table, session_, objectId_ );
    row.SetField( "danger_level", FieldVariant( nbc.danger_level() ) ); // int
    // nbc.nbc_agents // list object: TODO
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeCrossingSite& crossing_site )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Crossing_Site" ) );
    
    RowManipulator row( *table, session_, objectId_ );
    row.SetField( "banks_require_fitting", FieldVariant( crossing_site.banks_require_fitting() ) ); // int
    // nbc.nbc_agents // list object: TODO
    row.SetField( "depth", FieldVariant( crossing_site.depth() ) ); //int
    row.SetField( "flow_rate", FieldVariant( crossing_site.flow_rate() ) ); //int
    row.SetField( "width", FieldVariant( crossing_site.width() ) ); //int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeSupplyRoute& supply_route )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_SupplyRoute" ) );

    RowManipulator row( *table, session_, objectId_ );
    row.SetField( "equipped", FieldVariant( supply_route.equipped() ) ); // boolean
    row.SetField( "flow_rate", FieldVariant( supply_route.flow_rate() ) ); // int
    row.SetField( "length", FieldVariant( supply_route.length() ) ); // int
    row.SetField( "max_weight", FieldVariant( supply_route.max_weight() ) ); // int
    row.SetField( "width", FieldVariant( supply_route.width() ) ); // int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeToxicCloud& /*toxic_cloud*/ )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Toxic_Cloud" ) );
    
    RowManipulator row( *table, session_, objectId_ );
    //toxic_cloud.quantities // list: todo
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeFire& fire )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Fire" ) );
    RowManipulator row( *table, session_, objectId_ );
    
    row.SetField( "class_name", FieldVariant( fire.class_name() ) ); // string
    row.SetField( "max_combustion_energy", FieldVariant( fire.max_combustion_energy() ) ); // int
}

namespace
{
        
    // TODO all bed Capacities
    void UpdateBedCapacities( RowManipulator& row,  const sword::ObjectAttributeMedicalTreatment& medical_treatment )
    {
        int available = 0;
        bool availableUpdated = false;
        int baseline = 0;
        bool baselineUpdated = false;
        
        for ( int i = 0; i < medical_treatment.bed_capacities_size(); ++i ) 
        {
            const sword::MedicalTreatmentBedCapacity& capacity = medical_treatment.bed_capacities( i );
            // $$$$ JCR - HACK : Do not take into account NegativeFlowIsolation which is a subset of MedicalSurgical and the last registered element!! 
            // $$$$ TODO: ADD this information in the MedicalTreatment definition and the ASN message.
            if ( capacity.type_id() == 7 ) // SKIP
                continue;
            if( capacity.has_available_count() )
            {
                availableUpdated = true;
                available += capacity.available_count();
            }
            if( capacity.has_baseline_count() )
            {
                baselineUpdated = true;
                baseline += capacity.baseline_count();
            }
        }
        if( baselineUpdated )
            row.SetField( "beds", FieldVariant( baseline ) ); // int
        if( availableUpdated )
            row.SetField( "available_beds", FieldVariant( available ) ); // int
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeMedicalTreatment& medical_treatment )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Medical_Treatment" ) );

    RowManipulator row( *table, session_, objectId_ );
    
    if( medical_treatment.has_doctors() )
        row.SetField( "doctors", FieldVariant( static_cast< int >( medical_treatment.doctors() ) ) ); // int
    if( medical_treatment.has_available_doctors() )
        row.SetField( "available_doctors", FieldVariant( static_cast< int >( medical_treatment.available_doctors() ) ) ); // int
    UpdateBedCapacities( row, medical_treatment );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeInteractionHeight& interaction_height )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Interaction_Height" ) );

    RowManipulator row( *table, session_, objectId_ );
    row.SetField( "height", FieldVariant( interaction_height.height() ) ); // 
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeNBCType& nbc_agent )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_NBC_Type" ) );
    
    RowManipulator row( *table, session_, objectId_ );
    row.SetField( "agent_id", FieldVariant( static_cast< long >( nbc_agent.agent().id() ) ) ); // int
    row.SetField( "concentration", FieldVariant( nbc_agent.concentration() ) ); // int
    row.SetField( "source_life_duration", FieldVariant( nbc_agent.source_life_duration() ) ); // int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeStock& /*stock*/ )
{
    std::auto_ptr< Table_ABC > table( OpenTable( "TacticalObject_Attribute_Stock" ) );
    
    RowManipulator row( *table, session_, objectId_ );
    //todo: list : stock.resources
}
