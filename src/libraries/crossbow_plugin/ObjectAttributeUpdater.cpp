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
#include <gdal/ogr_core.h>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace plugins::crossbow;

class ObjectAttributeUpdater::RowUpdater_ABC
{
public:
    virtual long GetReferenceId( const std::string& attribute ) const = 0;
    virtual void SetProperty( long paramId, const std::string& type, const std::string& field, const std::string& value ) = 0;
};

namespace
{
    long GetRowId( Table_ABC& table, long objectId, const std::string& attribute )
    {
        Row_ABC* row = table.Find( "object_id=" + boost::lexical_cast< std::string >( objectId ) +
                                    " AND name='" + attribute + "' AND parameter_id is null" );
        if( row )
            return row->GetID();
        return OGRNullFID;
    }

    class RowInserter : public ObjectAttributeUpdater::RowUpdater_ABC
                      , boost::noncopyable
    {
    public:
        RowInserter( Workspace_ABC& workspace, long objectId )
            : workspace_ ( workspace )
            , objectId_ ( objectId )
        {
        }

        long GetReferenceId( const std::string& attribute ) const
        {
            std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "objectparameters" ) );
            Row_ABC& row = table->CreateRow();
            row.SetField( "object_id", FieldVariant( objectId_ ) );
            row.SetField( "type", FieldVariant( std::string( "List" ) ) );
            row.SetField( "name", FieldVariant( attribute ) );
            table->InsertRow( row );
            return GetRowId( *table, objectId_, attribute );
        }

        void SetProperty( long paramId, const std::string& type, const std::string& field, const std::string& value )
        {
            std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "objectparameters" ) );
            Row_ABC& row = table->CreateRow();
            row.SetField( "object_id", FieldVariant( objectId_ ) );
            row.SetField( "parameter_id", FieldVariant( paramId ) );
            row.SetField( "type", FieldVariant( type ) );
            row.SetField( "name", FieldVariant( field ) );
            row.SetField( "value", FieldVariant( value ) );
            table->InsertRow( row );
        }

    private:
        Workspace_ABC& workspace_;
        long objectId_;
    };

    class RowUpdater : public ObjectAttributeUpdater::RowUpdater_ABC
                     , boost::noncopyable
    {
    public:
        RowUpdater( Workspace_ABC& workspace, long objectId )
            : workspace_ ( workspace )
            , objectId_ ( objectId )
        {
            // NOTHING
        }

        long GetReferenceId( const std::string& attribute ) const
        {
            std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "objectparameters" ) );
            return GetRowId( *table, objectId_, attribute );
        }

        void SetProperty( long paramId, const std::string& /*type*/, const std::string& field, const std::string& value )
        {
            std::auto_ptr< Table_ABC > table( workspace_.GetDatabase( "geometry" ).OpenTable( "objectparameters" ) );

            Row_ABC* row = table->Find( "object_id=" + boost::lexical_cast< std::string >( objectId_ ) +
                                        " AND parameter_id=" + boost::lexical_cast< std::string >( paramId ) +
                                        " AND name='" + field + "'" );
            if( row )
            {
                row->SetField( "value", FieldVariant( value ) );
                table->UpdateRow( *row );
            }
        }

    private:
        Workspace_ABC& workspace_;
        long objectId_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater constructor
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
ObjectAttributeUpdater::ObjectAttributeUpdater( Workspace_ABC& workspace, long objectId )
{
    updater_.reset( new RowUpdater( workspace, objectId ) );
    inserter_.reset( new RowInserter( workspace, objectId ) );
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
    CHECK_ATTRIBUTE_UPDATE( life_time );
    CHECK_ATTRIBUTE_UPDATE( bypass );
    CHECK_ATTRIBUTE_UPDATE( construction );
    CHECK_ATTRIBUTE_UPDATE( crossing_site );
    CHECK_ATTRIBUTE_UPDATE( effect_delay );
    CHECK_ATTRIBUTE_UPDATE( fire );
    CHECK_ATTRIBUTE_UPDATE( interaction_height );
    CHECK_ATTRIBUTE_UPDATE( logistic );
    CHECK_ATTRIBUTE_UPDATE( medical_treatment );
    CHECK_ATTRIBUTE_UPDATE( mine );
    CHECK_ATTRIBUTE_UPDATE( nbc );
    CHECK_ATTRIBUTE_UPDATE( nbc_agent );
    CHECK_ATTRIBUTE_UPDATE( obstacle );
    CHECK_ATTRIBUTE_UPDATE( resource_networks );
    CHECK_ATTRIBUTE_UPDATE( stock );
    CHECK_ATTRIBUTE_UPDATE( supply_route );
    CHECK_ATTRIBUTE_UPDATE( toxic_cloud );
    CHECK_ATTRIBUTE_UPDATE( burn );
    CHECK_ATTRIBUTE_UPDATE( burn_surface );
    CHECK_ATTRIBUTE_UPDATE( flood );
    CHECK_ATTRIBUTE_UPDATE( lodging );
    CHECK_ATTRIBUTE_UPDATE( altitude_modifier );
}

/*
  select * from sword.objectparameters a,
   ( select distinct id from sword.objectparameters
        where object_id = 1 and name='Obstacle' and parameter_id is null ) b
     where a.parameter_id = b.id;
*/

namespace
{
    class RowManipulator : boost::noncopyable
    {
    public:
        RowManipulator( ObjectAttributeUpdater::RowUpdater_ABC& updater, ObjectAttributeUpdater::RowUpdater_ABC& inserter, const std::string& attribute )
            : updater_ ( &updater )
            , rowId_ ( updater.GetReferenceId( attribute ) )
        {
            if( rowId_ == OGRNullFID )
            {
                rowId_ = inserter.GetReferenceId( attribute );
                updater_ = &inserter;
            }
        }

        void SetProperty( const std::string& type, const std::string& field, const std::string& value )
        {
            updater_->SetProperty( rowId_, type, field, value );
        }

    private:
        long rowId_;
        ObjectAttributeUpdater::RowUpdater_ABC* updater_;
    };

    template< typename T >
    std::string ToString( const T& value )
    {
        return boost::lexical_cast< std::string >( value );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeConstruction& construction )
{
    RowManipulator row( *updater_, *inserter_, "Construction" );

    if( construction.density() )
        row.SetProperty( "Numeric", "density", ToString( construction.density() ) );
    if( construction.has_percentage() )
        row.SetProperty( "Quantity", "percentage", ToString( construction.percentage() ) ); //int
    if( construction.has_resource() )
        row.SetProperty( "Identifier", "resource_type", ToString( construction.resource().id() ) ); //int
    if( construction.has_dotation() )
        row.SetProperty( "Identifier", "dotation_type", ToString( construction.dotation() ) ); //int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeObstacle& obstacle )
{
    RowManipulator row( *updater_, *inserter_, "Obstacle" );

    if( obstacle.has_activated() )
        row.SetProperty( "Bool", "activated", ToString( obstacle.activated() ) ); // bool
    if( obstacle.has_activation_time() )
        row.SetProperty( "Quantity", "activation_time", ToString( obstacle.activation_time() ) ); // DateTime
    if( obstacle.has_type() )
        row.SetProperty( "String", "type", std::string( obstacle.type() == sword::ObstacleType_DemolitionTargetType_preliminary ? "preliminary" : "reserved" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeMine& mine )
{
    RowManipulator row( *updater_, *inserter_, "Mine" );

    if( mine.has_density() )
        row.SetProperty( "Numeric", "density", ToString( mine.density() ) );
    if( mine.has_percentage() )
        row.SetProperty( "Quantity", "percentage", ToString( mine.percentage() ) );
    if( mine.has_resource() )
        row.SetProperty( "Identifier", "dotation_type", ToString( mine.resource().id() ) );
    if( mine.has_dotation() )
        row.SetProperty( "Quantity", "dotation_nbr", ToString( mine.dotation() ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeLifeTime& life_time )
{
    RowManipulator row( *updater_, *inserter_, "ActivityTime" );

    if( life_time.has_value() )
        row.SetProperty( "Quantity", "activity_time", ToString( life_time.value() ) ); // DateTime
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeBypass& bypass )
{
    RowManipulator row( *updater_, *inserter_, "Bypass" );

    if( bypass.has_percentage() )
        row.SetProperty( "Quantity", "percentage", ToString( bypass.percentage() ) ); //int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeLogistic& logistic )
{
    RowManipulator row( *updater_, *inserter_, "Logistic" );

    if( logistic.logistic_superior().has_automat() )
        row.SetProperty( "Identifier", "tc2", ToString( logistic.logistic_superior().automat().id() ) ); // automat id
    else if( logistic.logistic_superior().has_formation() )
        row.SetProperty( "Identifier", "tc2", ToString( logistic.logistic_superior().formation().id() ) ); // automat id
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeNBC& nbc )
{
    RowManipulator row( *updater_, *inserter_, "NBC" );

    row.SetProperty( "Quantity", "danger_level", ToString( nbc.danger_level() ) ); // int
    // nbc.nbc_agents // list object: TODO
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeCrossingSite& crossing_site )
{
    RowManipulator row( *updater_, *inserter_, "CrossingSite" );

    row.SetProperty( "Bool", "banks_require_fitting", ToString( crossing_site.banks_require_fitting() ) );
    row.SetProperty( "Quantity", "depth", ToString( crossing_site.depth() ) );
    row.SetProperty( "Quantity", "flow_rate", ToString( crossing_site.flow_rate() ) );
    row.SetProperty( "Quantity", "width", ToString( crossing_site.width() ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeSupplyRoute& supply_route )
{
    RowManipulator row( *updater_, *inserter_, "SupplyRoute" );

    row.SetProperty( "Bool", "equipped", ToString( supply_route.equipped() ) ); // boolean
    row.SetProperty( "Quantity", "flow_rate", ToString( supply_route.flow_rate() ) ); // int
    row.SetProperty( "Quantity", "length", ToString( supply_route.length() ) ); // int
    row.SetProperty( "Quantity", "max_weight", ToString( supply_route.max_weight() ) ); // int
    row.SetProperty( "Quantity", "width", ToString( supply_route.width() ) ); // int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeToxicCloud& /*toxic_cloud*/ )
{
    RowManipulator row( *updater_, *inserter_, "ToxicCloud" );

    //toxic_cloud.quantities // list: todo
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeFire& fire )
{
    RowManipulator row( *updater_, *inserter_, "Fire" );

    row.SetProperty( "Identifier", "class_name", ToString( fire.class_name() ) ); // int
    row.SetProperty( "Quantity", "combustion_energy", ToString( fire.max_combustion_energy() ) ); // int
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
            if ( capacity.type_id() == 7 ) // SKIP NegativeFlowIsolation
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
            row.SetProperty( "Quantity", "beds", ToString( baseline ) ); // int
        if( availableUpdated )
            row.SetProperty( "Quantity", "available_beds", ToString( available ) ); // int
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeMedicalTreatment& medical_treatment )
{
    RowManipulator row( *updater_, *inserter_, "MedicalTreatment" );

    if( medical_treatment.has_doctors() )
        row.SetProperty( "Quantity", "doctors", ToString( medical_treatment.doctors() ) ); // int
    if( medical_treatment.has_available_doctors() )
        row.SetProperty( "Quantity", "available_doctors", ToString( medical_treatment.available_doctors() ) ); // int
    UpdateBedCapacities( row, medical_treatment );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeInteractionHeight& interaction_height )
{
    RowManipulator row( *updater_, *inserter_, "InteractionHeight" );

    row.SetProperty( "Numeric", "height", ToString( interaction_height.height() ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeNBCType& nbc_agent )
{
    RowManipulator row( *updater_, *inserter_, "NBCType" );

    row.SetProperty( "Identifier", "agent_id", ToString( nbc_agent.agent().id() ) ); // int
    row.SetProperty( "Quantity", "concentration", ToString( nbc_agent.concentration() ) ); // float
    row.SetProperty( "Quantity", "source_life_duration", ToString( nbc_agent.source_life_duration() ) ); // int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2010-06-08
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeStock& /*stock*/ )
{
    RowManipulator row( *updater_, *inserter_, "Stock" );

    //todo: list : stock.resources
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2011-01-24
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeEffectDelay& /*delay*/ )
{
    RowManipulator row( *updater_, *inserter_, "EffectDelay" );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2011-01-24
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeResourceNetwork& /*network*/ )
{
    RowManipulator row( *updater_, *inserter_, "ResourceNetwork" );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2011-01-26
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeBurn& burn )
{
    RowManipulator row( *updater_, *inserter_, "Burn" );

    row.SetProperty( "Quantity", "heat", ToString( burn.current_heat() ) ); // int
    row.SetProperty( "Quantity", "combustion_energy", ToString( burn.combustion_energy() ) ); // int
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2011-01-26
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeBurnSurface& /*burn_surface*/ )
{
    RowManipulator row( *updater_, *inserter_, "BurnSurface" );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JCR 2011-01-26
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeFlood& /*flood*/ )
{
    RowManipulator row( *updater_, *inserter_, "Flood" );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeLodging& /*lodging*/ )
{
    RowManipulator row( *updater_, *inserter_, "Lodging" );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeUpdater::Update
// Created: JSR 2011-05-17
// -----------------------------------------------------------------------------
void ObjectAttributeUpdater::Update( const sword::ObjectAttributeAltitudeModifier& /*altitude_modifier*/ )
{
    RowManipulator row( *updater_, *inserter_, "AltitudeModifier" );

}
