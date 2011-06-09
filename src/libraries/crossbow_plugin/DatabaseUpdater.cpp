// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "protocol/Protocol.h"
#include "DatabaseUpdater.h"
#include "ObjectAttributeUpdater.h"
#include "ResourceNetworkUpdater.h"
#include "Database_ABC.h"
#include "Workspace_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "Line.h"
#include "Point.h"
#include "Area.h"
#include "dispatcher/EntitySymbols_ABC.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Object.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/ObjectKnowledge.h"
#include "dispatcher/KnowledgeGroup.h"
#include "dispatcher/Side.h"
#include "clients_kernel/ObjectType.h"
#include "tools/App6Symbol.h"
#include "WorkingSession_ABC.h"

#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace plugins::crossbow;

namespace plugins
{
namespace crossbow
{
    class DatabaseUpdater::LazyDatabaseConnection : public boost::noncopyable
    {
    public:
        LazyDatabaseConnection( Workspace_ABC& workspace )
            : workspace_ ( workspace ), geometry_ ( 0 ) {}

        void Reset()
        {
            GetGeometry();
            Flush( true );
        }

        void Flush( bool reset )
        {
            if( geometry_ )
            {
                geometry_->Flush();
                if( reset )
                {
                    workspace_.Release( "geometry" );
                    geometry_ = 0;
                }
            }
        }

        Database_ABC& GetGeometry()
        {
            geometry_ = &workspace_.GetDatabase( "geometry" );
            return *geometry_;
        }

    private:
        Workspace_ABC& workspace_;
        Database_ABC* geometry_;
    };
}
}


// -----------------------------------------------------------------------------
// Name: DatabaseUpdater constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabaseUpdater::DatabaseUpdater( Workspace_ABC& workspace, const dispatcher::Model_ABC& model, const WorkingSession_ABC& session )
    : workspace_ ( workspace )
    , database_ ( new LazyDatabaseConnection( workspace ) )
    , model_    ( model )
    , session_  ( session )
{
    database_->Reset();
    Clean();
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabaseUpdater::~DatabaseUpdater()
{
    // NOTHING
}

namespace
{
    std::string FormatSymbol( const std::string& symbol )
    {
        std::string result( symbol.size(), '*' );
        std::transform( symbol.begin(), symbol.end(), result.begin(), toupper );
        std::replace( result.begin(), result.end(), '*', '-' );
        if( result.size() < 15 )
            result.append( std::string( 15 - symbol.size(), '-' ) );
        return result;
    }

    template< typename List >
    void UpdateSymbol( Row_ABC& row, List& list, int id )
    {
        if( const kernel::Entity_ABC* element = list.Find( id ) )
            row.SetField( "symbol_id", FieldVariant( FormatSymbol( element->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Flush
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseUpdater::Flush( bool reset /*= true*/  )
{
    database_->Flush( reset );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Clean
// Created: JCR 2010-03-01
// -----------------------------------------------------------------------------
void DatabaseUpdater::Clean()
{
    try
    {
        const std::string clause( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) );

        database_->GetGeometry().ClearTable( "UnitForces", clause );
        database_->GetGeometry().ClearTable( "KnowledgeUnits", clause );
        database_->GetGeometry().ClearTable( "BoundaryLimits", clause );
        database_->GetGeometry().ClearTable( "TacticalLines", clause );

        database_->GetGeometry().ClearTable( "Objects", clause );
        database_->GetGeometry().ClearTable( "KnowledgeObjects", clause );

        database_->GetGeometry().ClearTable( "Formations", clause );
        database_->GetGeometry().ClearTable( "SimulationClock", clause );
        database_->GetGeometry().ClearTable( "Teams", clause );
        database_->GetGeometry().ClearTable( "urban_blocks", clause );
        database_->GetGeometry().ClearTable( "resource_network", clause );
        database_->GetGeometry().ClearTable( "inhabitants", clause );
        database_->GetGeometry().ClearTable( "inhabitants_urban_blocks_occupation", clause );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "DatabaseUpdater is not correctly loaded : " + std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::UnitCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "UnitForces" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.unit().id() ) );
    row.SetField( "parent_oid", FieldVariant( ( long ) msg.automat().id() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.name() ) ) );
    row.SetField( "type"      , FieldVariant( ( long ) msg.type().id() ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Agents(), msg.unit().id() );
    row.SetGeometry( Point() );
    table->InsertRow( row );
}

namespace
{
    std::string InverseAffiliation( const std::string& affiliation )
    {
        if( affiliation == "H" ) return "F";
        if( affiliation == "F" ) return "H";
        return affiliation;
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::UnitKnowledgeCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "KnowledgeUnits" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid"  , FieldVariant( ( long ) msg.knowledge().id() ) );
    row.SetField( "group_oid"   , FieldVariant( ( long ) msg.knowledge_group().id() ) );
    row.SetField( "unit_oid", FieldVariant( ( long ) msg.unit().id() ) );
    row.SetField( "type", FieldVariant( ( long ) msg.type().id() ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    if( const dispatcher::Agent_ABC* realAgent = model_.Agents().Find( msg.unit().id() ) )
    {
        std::string a = tools::app6::GetAffiliation( realAgent->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() );
        row.SetField( "observer_affiliation", FieldVariant( InverseAffiliation( a ) ) );
    }
    row.SetField( "symbol_id", FieldVariant( FormatSymbol( "SUZP********---" ) ) ); // $$$$ SBO 2007-09-27: hard coded...
    row.SetGeometry( Point() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: AME 2009-10-12
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::ObjectKnowledgeCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "KnowledgeObjects" ) );

    const dispatcher::ObjectKnowledge_ABC* knowledge = model_.ObjectKnowledges().Find( msg.knowledge().id() );
    const kernel::Object_ABC* entity = knowledge->GetEntity();
    if( !entity ) // $$$$ _RC_ SBO 2010-06-10: no real object => giving up
        return;
    std::string symbol = entity->GetType().GetSymbol();
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( (long) msg.knowledge().id() ) );
    row.SetField( "type", FieldVariant( std::string( msg.type().id() ) ) );
    row.SetField( "team_id", FieldVariant( (long)msg.party().id() ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );

    if( msg.attributes().has_construction() )
        row.SetField( "state", FieldVariant( msg.attributes().construction().percentage() ) );
    if( tools::app6::GetAffiliation( symbol ) != "U" )
        row.SetField( "name", FieldVariant( std::string( entity->GetName().ascii() ) ) );
    if( const dispatcher::KnowledgeGroup* knowledgeGroup = static_cast< const dispatcher::KnowledgeGroup* >( model_.KnowledgeGroups().Find( msg.party().id() ) ) )
    {
        tools::app6::SetAffiliation( symbol, (unsigned int) knowledgeGroup->GetTeam().GetKarma().GetUId() );
        row.SetField( "observer_affiliation", FieldVariant( tools::app6::GetAffiliation( symbol ) ) );
    }
    else
        tools::app6::SetAffiliation( symbol, (unsigned int) sword::unknown );
    row.SetField( "symbol_id", FieldVariant( FormatSymbol( symbol ) ) );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::LimitCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "BoundaryLimits" ) );

    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.id().id() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.tactical_line().name() ) ) );
    row.SetField( "symbol_id" , FieldVariant( std::string( "G-GPGLB----H--X" ) ) ); // default
    row.SetField( "session_id", session_.GetId() );
    row.SetGeometry( Line( msg.tactical_line().geometry().coordinates() ) );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::PhaseLineCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "TacticalLines" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( (long) msg.id().id() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.tactical_line().name() ) ) );
    row.SetField( "session_id", session_.GetId() );
    row.SetGeometry( Line( msg.tactical_line().geometry().coordinates() ) );
    table->InsertRow( row );
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: QueryDatabaseUpdater::UpdateGeometry
    // Created: JCR 2009-11-02
    // -----------------------------------------------------------------------------
    bool UpdateGeometry( Row_ABC& row, const sword::Location& location )
    {
         bool result = false;
        switch ( location.type() )
        {
        case sword::Location::point:
            row.SetGeometry( Point( location.coordinates().elem( 0 ) ) );
            result = true;
            break;
        case sword::Location::line:
            row.SetGeometry( Line( location.coordinates() ) );
            result = true;
            break;
        case sword::Location::polygon:
            result = true;
            row.SetGeometry( Area( location.coordinates() ) );
            break;
        default:                      /*row.SetGeometry( Location( asn.location ) );*/ break; // $$$$ SBO 2007-08-31: TODO
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::UpdateObjectAttributes
// Created: JCR 2010-07-21
// -----------------------------------------------------------------------------
void DatabaseUpdater::UpdateObjectAttributes( long objectId, const sword::ObjectAttributes& msg )
{
    ObjectAttributeUpdater updater( workspace_, objectId );

    updater.Update( msg );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::ObjectCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "Objects" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( (long) msg.object().id() ) );
    row.SetField( "name" , FieldVariant( std::string( msg.name() ) ) );
    row.SetField( "type" , FieldVariant( std::string( msg.type().id() ) ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Objects(), msg.object().id() );
    UpdateGeometry( row, msg.location() );
    table->InsertRow( row );

    long rowId = row.GetID();
    if( rowId == OGRNullFID )
    {
        Row_ABC* row = table->Find( "public_oid=" + boost::lexical_cast< std::string >( (long) msg.object().id() ) + " AND session_id=" +
                                     boost::lexical_cast< std::string >( session_.GetId() ) );
        if( row )
            rowId = row->GetID();
    }
    if( msg.has_attributes() )
        UpdateObjectAttributes( rowId, msg.attributes() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: JCR 2010-05-12
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::ObjectUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "Objects" ) );

    std::stringstream query;
    query << "public_oid=" << msg.object().id() << " AND session_id=" << session_.GetId();
    Row_ABC* row = table->Find( query.str() );

    if( row )
    {
        if( msg.has_location() && msg.location().coordinates().elem_size() > 0 )
        {
            if( UpdateGeometry( *row, msg.location() ) )
                table->UpdateRow( *row );
        }
        if( msg.has_attributes() )
            UpdateObjectAttributes( row->GetID(), msg.attributes() );
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// Modified: MPT 2009-09-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::ObjectKnowledgeUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "KnowledgeObjects" ) );

    std::stringstream query;
    query << "public_oid=" << msg.knowledge().id() << " AND session_id=" << session_.GetId();

    if( Row_ABC* row = table->Find( query.str() ) )
    {
        row->SetField( "team_id", FieldVariant( (long)msg.party().id() ) );
        if( msg.has_attributes() && msg.attributes().has_construction() )
            row->SetField( "state", FieldVariant( msg.attributes().construction().percentage() ) );

        const dispatcher::ObjectKnowledge_ABC* knowledge = model_.ObjectKnowledges().Find( msg.knowledge().id() );
        const kernel::Object_ABC* entity = knowledge->GetEntity();
        if( !entity ) // $$$$ _RC_ SBO 2010-06-10: no real object => giving up
            return;
        std::string symbol = entity->GetType().GetSymbol();
        if( tools::app6::GetAffiliation( symbol ) != "U" )
            row->SetField( "name",  FieldVariant( std::string( entity->GetName() ) ) );

        if( const dispatcher::KnowledgeGroup* knowledgeGroup = static_cast< const dispatcher::KnowledgeGroup* >( model_.KnowledgeGroups().Find( msg.party().id() ) ) )
        {
            tools::app6::SetAffiliation( symbol, (unsigned int) knowledgeGroup->GetTeam().GetKarma().GetUId() );
            row->SetField( "observer_affiliation", FieldVariant( tools::app6::GetAffiliation( symbol ) ) );
        }
        else
            tools::app6::SetAffiliation( symbol, (unsigned int) sword::unknown );

        row->SetField( "symbol_id", FieldVariant( FormatSymbol( symbol ) ) );
        //Geometry
        if( msg.has_location() )
            UpdateGeometry( *row, msg.location() );

        table->UpdateRow( *row );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::FormationCreation& message )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "Formations" ) );

    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) message.formation().id() ) );
    if( message.has_parent() )
        row.SetField( "parent_oid", FieldVariant( ( long ) message.parent().id() ) );
    else
        row.SetField( "parent_oid", FieldVariant( ( long ) 0 ) );
    row.SetField( "name", FieldVariant( std::string( message.name() ) ) );
    row.SetField( "type", FieldVariant( -1 ) );
    row.SetField( "engaged", FieldVariant( 0 ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Formations(), message.formation().id() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::AutomatCreation& message )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "Formations" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) message.automat().id() ) );
    if( message.parent().has_formation() )
        row.SetField( "parent_oid", FieldVariant( ( long ) message.parent().formation().id() ) );
    else
        row.SetField( "parent_oid", FieldVariant( ( long ) message.parent().automat().id() ) );
    row.SetField( "name", FieldVariant( std::string( message.name() ) ) );
    row.SetField( "type", FieldVariant( ( long ) message.type().id() ) );
    row.SetField( "engaged", FieldVariant( true ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Automats(), ( long ) message.automat().id() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: JCR 2008-07-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::AutomatAttributes& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "Formations" ) );
    std::stringstream query;
    query << "public_oid=" << msg.automat().id() << " AND session_id=" << session_.GetId();

    if( Row_ABC* row = table->Find( query.str() ) )
    {
        if( msg.has_mode() )
            row->SetField( "engaged", FieldVariant( ( msg.mode() == sword::engaged ) ? -1 : 0 ) );
        table->UpdateRow( *row );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
/* Update and insert cursors are bulk data loading and data update API's
   de-signed for performing direct updates and inserts, outside of an edit
   session, on simple data, during the data loading phase of a project.
   Avoid using these API's in editing applications. Using these API's within an
   edit session or on complex objects (objects with non-simple row or feature
   behavior or on objects participating in composite relationships or
   relationships with notifica-tion) negates any performance advantages they
   may have

   http://edndoc.esri.com/arcobjects/8.3/ComponentHelp/esriCore/IWorkspaceEdit.htm
*/
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::UnitAttributes& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.unit().id() << " AND session_id=" << session_.GetId();

    if( Row_ABC* row = table->Find( query.str() ) )
    {
        if( msg.has_speed() )
            row->SetField( "speed", FieldVariant( msg.speed() ) );
        if( msg.has_raw_operational_state() )
            row->SetField( "op_state", FieldVariant( msg.raw_operational_state() ) );
        if( msg.has_direction() )
            row->SetField( "direction", FieldVariant( msg.direction().heading() ) );
        if( msg.has_position() )
            row->SetGeometry( Point( msg.position() ) );
        table->UpdateRow( *row );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::UnitKnowledgeUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "KnowledgeUnits" ) );
    std::stringstream query;
    query << "public_oid=" << msg.knowledge().id() << " AND session_id=" << session_.GetId();

    //AME 2009-10-12 TODO: update field observer_oid
    if( Row_ABC* row = table->Find( query.str() ) )
    {
        if( msg.has_speed() )
            row->SetField( "speed", FieldVariant( msg.speed() ) );
        if( msg.has_direction() )
            row->SetField( "direction", FieldVariant( msg.direction().heading() ) );
        if( msg.has_identification_level() )
            row->SetField( "identification_level", FieldVariant( msg.identification_level() ) );
        if( msg.has_dead() )
            row->SetField( "dead", FieldVariant( msg.dead() ) );
        if( msg.has_max_identification_level() )
            UpdateSymbol( *row, model_.AgentKnowledges(), msg.knowledge().id() );
        if( msg.has_position() )
            row->SetGeometry( Point( msg.position() ) );
        table->UpdateRow( *row );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyUnit
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::UnitDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.unit().id() << " AND session_id=" << session_.GetId();
    table->DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::ObjectDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "Objects" ) );
    std::stringstream query;
    query << "public_oid=" << msg.object().id() << " AND session_id=" << session_.GetId();
    table->DeleteRows( query.str()  );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::UnitKnowledgeDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "KnowledgeUnits" ) );
    std::stringstream query;
    query << "public_oid=" << msg.knowledge().id() << " AND session_id=" << session_.GetId();
    table->DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::ObjectKnowledgeDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "KnowledgeObjects" ) );
    std::stringstream query;
    query << "public_oid=" << msg.knowledge().id() << " AND session_id=" << session_.GetId();
    table->DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: MPT 2009-12-15
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::ControlBeginTick& msg )
{
    std::stringstream query;

    query << "session_id=" << session_.GetId();

    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "SimulationClock" ) );

    Row_ABC* row = table->Find( query.str() );
    std::string datetime = msg.date_time().data();
    if( !row ) // Do not exist yet
    {
        Row_ABC& insert = table->CreateRow();

        insert.SetField( "clock", FieldVariant( datetime ) );
        insert.SetField( "session_id", session_.GetId() );
        table->InsertRow( insert );
    }
    else
    {
        row->SetField( "clock", FieldVariant( datetime ) );
        table->UpdateRow( *row );
    }
}


// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: MPT 2009-12-15
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::PartyCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "Teams" ) );

    Row_ABC& row = table->CreateRow();

    row.SetField( "public_oid", FieldVariant( static_cast< long >( msg.party().id() ) ) );
    row.SetField( "name", FieldVariant( msg.name() ) );
    row.SetField( "type", FieldVariant( msg.type() ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    table->InsertRow( row );

    /* ASN1T_MsgTeamCreation.type:
    struct EXTERN EnumDiplomacy {
       enum Root {
          inconnu = 0,
          ami = 1,
          ennemi = 2,
          neutre = 3
       } ;
    } ;*/
}

namespace
{
    std::string MakeColor( const sword::RgbaColor& color )
    {
        return "(" + boost::lexical_cast< std::string >( color.red() ) + ","
                   + boost::lexical_cast< std::string >( color.green() ) + ","
                   + boost::lexical_cast< std::string >( color.blue() ) + ","
                   + boost::lexical_cast< std::string >( color.alpha() ) + ")";
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::UpdateUrbanBlockAttributes
// Created: JCR 2011-01-25
// -----------------------------------------------------------------------------
void DatabaseUpdater::UpdateUrbanBlockAttributes( Row_ABC& row, const sword::UrbanAttributes& attributes )
{
    if( attributes.has_structure() )
        row.SetField( "structure_state", FieldVariant( static_cast< int >( attributes.structure().state() ) ) );
    if( attributes.has_architecture() )
    {
        row.SetField( "archi_height",  FieldVariant( attributes.architecture().height() ) );
        row.SetField( "archi_floor_number",  FieldVariant( attributes.architecture().floor_number() ) );
        row.SetField( "archi_roof_shape",  FieldVariant( attributes.architecture().roof_shape() ) );
        row.SetField( "archi_material",  FieldVariant( attributes.architecture().material() ) );
        row.SetField( "archi_occupation",  FieldVariant( attributes.architecture().occupation() ) );
        row.SetField( "archi_trafficability",  FieldVariant( attributes.architecture().trafficability() ) );
        // 4.2.2 row.SetField( "archi_parking_available",  FieldVariant( msg.attributes().architecture().parking_available() ) );
    }
    if( attributes.has_color() )
        row.SetField( "color", FieldVariant( MakeColor( attributes.color() ) ) );

    //4.2.2
    /*if( attributes.has_infrastructures() )
    {

        row.SetField( "infra_type", FieldVariant( attributes.) );
        row.SetField( "infra_active", FieldVariant() );
        row.SetField( "infra_threshold", FieldVariant() );
        row.SetField( "usage_role", FieldVariant() );
        row.SetField( "usage_percentage", FieldVariant() );
    }*/

}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::UpdateResourceNetworks
// Created: JCR 2011-01-25
// -----------------------------------------------------------------------------
void DatabaseUpdater::UpdateResourceNetworks( const sword::UrbanAttributes_Infrastructures& infra, long oid, bool creation )
{
    if( infra.resource_network_size() != 0 )
    {
        for( int i = 0; i < infra.resource_network_size(); i++ )
            UpdateResourceNetwork( infra.resource_network( i ), oid, session_.GetId(), creation );
    }
}
// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: JCR 2011-01-25
// -----------------------------------------------------------------------------
void DatabaseUpdater::UpdateResourceNetwork( const sword::ResourceNetwork& net, long oid, int session_id, bool creation )
{
    ResourceNetworkUpdater netUpdater( workspace_, oid, session_id, creation );
    netUpdater.Update( net );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: JCR 2011-01-25
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::UrbanCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "urban_blocks" ) );

    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( static_cast< long >( msg.object().id() ) ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    if( msg.has_parent() )
        row.SetField( "parent_oid", FieldVariant( static_cast< long >( msg.parent().id() ) ) );
    if( msg.has_name() )
        row.SetField( "name", FieldVariant( msg.name() ) );
    if( msg.has_attributes() )
        UpdateUrbanBlockAttributes( row, msg.attributes() );
    if( msg.has_location() )
        UpdateGeometry( row, msg.location() );
    table->InsertRow( row );
    table.reset();
    //create network resource (one table per instance can be open )
    if( msg.attributes().has_infrastructures() )
        UpdateResourceNetworks( msg.attributes().infrastructures(), static_cast< long >( msg.object().id() ), true );

}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: JCR 2011-01-25
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::UrbanUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "urban_blocks" ) );

    Row_ABC* row = table->Find( "public_oid=" + boost::lexical_cast< std::string >( msg.object().id() ) +
                                   " AND session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) );
    if( row )
    {
        if( msg.has_attributes() )
            UpdateUrbanBlockAttributes( *row, msg.attributes() );
        if( msg.has_location() )
            UpdateGeometry( *row, msg.location() );
        table->UpdateRow( *row );
        table.reset();
        //create network resource (one table per instance can be open )
        if( msg.attributes().has_infrastructures() )
            UpdateResourceNetworks( msg.attributes().infrastructures(), static_cast< long >( msg.object().id() ), false );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: BCI 2011-06-08
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::PopulationCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "inhabitants" ) );

    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( static_cast< long >( msg.id().id() ) ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    row.SetField( "name", FieldVariant( msg.name() ) );
    row.SetField( "team_id", FieldVariant( (long)msg.party().id() ) );
    table->InsertRow( row );
    table.reset();
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: BCI 2011-06-09
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const sword::PopulationUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "inhabitants_urban_blocks_occupation" ) );

    std::string whereClause = "inhabitant_id=" + boost::lexical_cast< std::string >( msg.id().id() )
                            + " AND session_id=" + boost::lexical_cast< std::string >( session_.GetId() )
                            + " AND urban_block_id=";

    for( int i = 0, count = msg.occupations_size(); i<count; ++i )
    {
        const sword::PopulationUpdate_BlockOccupation& occupation = msg.occupations( i );
        Row_ABC* row = table->Find( whereClause + boost::lexical_cast< std::string >( occupation.object().id() ) );
        if( row )
        {
            UpdateBlockOccupation( *row, occupation );
            table->UpdateRow( *row );
        }
        else
        {
            Row_ABC& newRow = table->CreateRow();
            newRow.SetField( "inhabitant_id", FieldVariant( static_cast< long >( msg.id().id() ) ) );
            newRow.SetField( "urban_block_id", FieldVariant( static_cast< long >( occupation.object().id() ) ) );
            newRow.SetField( "session_id", FieldVariant( session_.GetId() ) );

            UpdateBlockOccupation( newRow, occupation );
            table->InsertRow( newRow );

        }
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::UpdateBlockOccupation
// Created: BCI 2011-06-09
// -----------------------------------------------------------------------------
void DatabaseUpdater::UpdateBlockOccupation( Row_ABC& row, const sword::PopulationUpdate_BlockOccupation& occupation )
{
    row.SetField( "alerted",  FieldVariant( std::string( occupation.alerted() ? "true" : "false" ) ) );
    row.SetField( "confined",  FieldVariant( std::string( occupation.confined() ? "true" : "false" ) ) );
    row.SetField( "evacuated",  FieldVariant( std::string( occupation.evacuated() ? "true" : "false" ) ) );
    row.SetField( "angriness",  FieldVariant( std::string( occupation.angriness() ? "true" : "false" ) ) );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Log
// Created: MPT 2009-12-22
// -----------------------------------------------------------------------------
void DatabaseUpdater::Log( const sword::ObjectMagicActionAck& msg )
{
    // TODO (MPT): a rapid hack. Should extract Log function and allow to either dump to DB or write to console
    if( msg.error_code() != sword::ObjectMagicActionAck::no_error )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__ + std::string( ": ObjectMagicActionAck indicates error: " ) + Error( msg.error_code() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Error
// Created: MPT 2009-12-22
// -----------------------------------------------------------------------------
std::string DatabaseUpdater::Error( const sword::ObjectMagicActionAck_ErrorCode& error_code ) const
{
    switch ( error_code )
    {
    case sword::ObjectMagicActionAck::no_error:
        return "no_error";
    case sword::ObjectMagicActionAck::error_invalid_party:
        return "error_invalid_party";
    case sword::ObjectMagicActionAck::error_invalid_object:
        return "error_invalid_object";
    case sword::ObjectMagicActionAck::error_invalid_specific_attributes:
        return "error_invalid_specific_attributes";
    default:
        return "unknown error code";
    }
}
