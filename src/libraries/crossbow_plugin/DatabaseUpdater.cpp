// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "protocol/protocol.h"
#include "DatabaseUpdater.h"
#include "ObjectAttributeUpdater.h"
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
#include "QueryBuilder.h"
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

        Database_ABC& GetFlat()
        {
            return workspace_.GetDatabase( "flat" );
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
        database_->GetGeometry().ClearTable( "TacticalObject_Area", clause );
        database_->GetGeometry().ClearTable( "TacticalObject_Line", clause );
        database_->GetGeometry().ClearTable( "TacticalObject_Point", clause );
        
        database_->GetFlat().ClearTable( "KnowledgeObjects", clause );
        database_->GetFlat().ClearTable( "Reports", clause );
        database_->GetFlat().ClearTable( "Formations", clause );
        database_->GetFlat().ClearTable( "SimulationClock", clause );
        database_->GetFlat().ClearTable( "Teams", clause );

        database_->GetFlat().ClearTable( "TacticalObject_Attribute_activity_time", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_bypass", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_construction", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_crossing_site", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_fire", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_interaction_height", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_logistic", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_medical_treatment", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_mine", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_nbc", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_nbc_type", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_obstacle", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_stock", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_supplyroute", clause );
        database_->GetFlat().ClearTable( "TacticalObject_Attribute_toxic_cloud", clause );
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "QueryDatabaseUpdater is not correctly loaded : " + std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "UnitForces" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.unit().id() ) );
    row.SetField( "parent_oid", FieldVariant( ( long ) msg.automat().id() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.nom() ) ) );
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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "KnowledgeUnits" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid"  , FieldVariant( ( long ) msg.knowledge().id() ) );
    row.SetField( "group_oid"   , FieldVariant( ( long ) msg.knowledge_group().id() ) );
    row.SetField( "unit_oid", FieldVariant( ( long ) msg.unit().id() ) );
   // $$$$ NEEDED ? builder.SetField( "type", msg.type_unite );
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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetFlat().OpenTable( "KnowledgeObjects" ) );

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
        tools::app6::SetAffiliation( symbol, (unsigned int) Common::unknown_diplo );
    row.SetField( "symbol_id", FieldVariant( FormatSymbol( symbol ) ) );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsMessengerToClient::MsgLimitCreation& msg )
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
void DatabaseUpdater::Update( const MsgsMessengerToClient::MsgLimaCreation& msg )
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
    std::string GetObjectTable( const Common::MsgLocation& location )
    {
        std::string className = "TacticalObject_Area";
        switch ( location.type() )
        {
        case Common::MsgLocation::point: className = "TacticalObject_Point"; break;
        case Common::MsgLocation::line:  className = "TacticalObject_Line"; break;
        }
        return className;
    }

    std::string GetObjectKnowledgeTable( const Common::MsgLocation& location )
    {
        std::string className = "KnowledgeObjects_Area";
        switch ( location.type() )
        {
        case Common::MsgLocation::point: className = "KnowledgeObjects_Point"; break;
        case Common::MsgLocation::line:  className = "KnowledgeObjects_Line"; break;
        }
        return className;
    }

    // -----------------------------------------------------------------------------
    // Name: QueryDatabaseUpdater::UpdateGeometry
    // Created: JCR 2009-11-02
    // -----------------------------------------------------------------------------
    bool UpdateGeometry( Row_ABC& row, const Common::MsgLocation& location )
    {
         bool result = false;
        switch ( location.type() )
        {
        case Common::MsgLocation::point:    
            row.SetGeometry( Point( location.coordinates().elem( 0 ) ) ); 
            result = true;
            break;
        case Common::MsgLocation::line:     
            row.SetGeometry( Line( location.coordinates() ) ); 
            result = true;
            break;
        case Common::MsgLocation::polygon:  
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
void DatabaseUpdater::UpdateObjectAttributes( unsigned long oid, const Common::ObjectAttributes& msg )
{
    ObjectAttributeUpdater updater( workspace_, session_, oid );

    updater.Update( msg );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( GetObjectTable( msg.location() ) ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( (long) msg.object().id() ) );
    row.SetField( "name" , FieldVariant( std::string( msg.name() ) ) );
    row.SetField( "type" , FieldVariant( std::string( msg.type().id() ) ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Objects(), msg.object().id() );
    UpdateGeometry( row, msg.location() );
    UpdateObjectAttributes( msg.object().id(), msg.attributes() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: JCR 2010-05-12
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectUpdate& msg )
{
    if ( msg.has_location() )
    {
        std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( GetObjectTable( msg.location() ) ) );

        std::stringstream query;
        query << "public_oid=" << msg.object().id() << " AND session_id=" << session_.GetId();
        if( Row_ABC* row = table->Find( query.str() ) )
        {
            if( msg.location().coordinates().elem_size() > 0 )
            {
                if( UpdateGeometry( *row, msg.location() ) )
                    table->UpdateRow( *row );
            }
        }
    }
    UpdateObjectAttributes( msg.object().id(), msg.attributes() );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// Modified: MPT 2009-09-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetFlat().OpenTable( "KnowledgeObjects" ) );

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
            tools::app6::SetAffiliation( symbol, (unsigned int) Common::unknown_diplo );

        row->SetField( "symbol_id", FieldVariant( FormatSymbol( symbol ) ) );
        //Geometry
        if( msg.has_location() )
            UpdateObjectKnowledgeGeometry( GetObjectKnowledgeTable( msg.location() ), msg );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::UpdateObjectKnowledgeGeometry
// Created: JCR 2009-11-02
// -----------------------------------------------------------------------------
void DatabaseUpdater::UpdateObjectKnowledgeGeometry( const std::string& tablename, const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( tablename ) );
    std::stringstream query;
    query << "public_oid=" << msg.knowledge().id() << " AND session_id=" << session_.GetId();

    Row_ABC* row = table->Find( query.str() );
    if( row == NULL )
    {
        row = &table->CreateRow();
        row->SetField( "public_oid", FieldVariant( (long) msg.knowledge().id() ) );
        row->SetField( "session_id", FieldVariant( session_.GetId() ) );
    }
    UpdateGeometry( *row, msg.location() );
}



// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgFormationCreation& message )
{
    std::auto_ptr< Table_ABC > table( database_->GetFlat().OpenTable( "Formations" ) );

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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgAutomatCreation& message )
{
    std::auto_ptr< Table_ABC > table( database_->GetFlat().OpenTable( "Formations" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) message.automat().id() ) );
    if( message.parent().has_formation() )
        row.SetField( "parent_oid", FieldVariant( ( long ) message.parent().formation().id() ) );
    else
        row.SetField( "parent_oid", FieldVariant( ( long ) message.parent().automat().id() ) );
    row.SetField( "name", FieldVariant( std::string( message.nom() ) ) );
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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgAutomatAttributes& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetFlat().OpenTable( "Formations" ) );
    std::stringstream query;
    query << "public_oid=" << msg.automat().id() << " AND session_id=" << session_.GetId();

    table->EndTransaction();
    if( Row_ABC* row = table->Find( query.str() ) )
    {
        if( msg.has_etat_automate() )
            row->SetField( "engaged", FieldVariant( ( msg.etat_automate() == Common::embraye ) ? -1 : 0 ) );
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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitAttributes& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.unit().id() << " AND session_id=" << session_.GetId();

    if( Row_ABC* row = table->Find( query.str() ) )
    {
        if( msg.has_vitesse() )
            row->SetField( "speed", FieldVariant( msg.vitesse() ) );
        if( msg.has_etat_operationnel_brut() )
            row->SetField( "op_state", FieldVariant( msg.etat_operationnel_brut() ) );
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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg )
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
        if( msg.has_mort() )
            row->SetField( "dead", FieldVariant( msg.mort() ) );
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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitDestruction& msg )
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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectDestruction& msg )
{
    std::stringstream ssQuery;
    ssQuery << "public_oid=" << msg.object().id() << " AND session_id=" << session_.GetId();
    std::string query( ssQuery.str() );
    {
        std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "TacticalObject_Point" ) );
        table->DeleteRows( query );
    }
    {
        std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "TacticalObject_Line" ) );
        table->DeleteRows( query );
    }
    {
        std::auto_ptr< Table_ABC > table( database_->GetGeometry().OpenTable( "TacticalObject_Area" ) );
        table->DeleteRows( query );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg )
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
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectKnowledgeDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetFlat().OpenTable( "KnowledgeObjects" ) );
    std::stringstream query;
    query << "public_oid=" << msg.knowledge().id() << " AND session_id=" << session_.GetId();
    table->DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: MPT 2009-12-15
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgControlBeginTick& msg )
{
    std::stringstream query;

    query << "session_id=" << session_.GetId();

    std::auto_ptr< Table_ABC > table( database_->GetFlat().OpenTable( "SimulationClock" ) );

    Row_ABC* row = table->Find( query.str() );
    if( !row ) // Do not exist yet
    {
        Row_ABC& insert = table->CreateRow();
        insert.SetField( "clock", FieldVariant( std::string( msg.date_time().data(), 15 ) ) );
        insert.SetField( "session_id", session_.GetId() );
        table->InsertRow( insert );
    }
    else
    {
        row->SetField( "clock", FieldVariant( std::string( msg.date_time().data(), 15 ) ) );
        table->UpdateRow( *row );
    }
}


// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: MPT 2009-12-15
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgPartyCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_->GetFlat().OpenTable( "Teams" ) );

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

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Log
// Created: MPT 2009-12-22
// -----------------------------------------------------------------------------
void DatabaseUpdater::Log( const MsgsSimToClient::MsgObjectMagicActionAck& msg )
{
    // TODO (MPT): a rapid hack. Should extract Log function and allow to either dump to DB or write to console
    if( msg.error_code() != MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_no_error )
    {
        MT_LOG_ERROR_MSG( __FUNCTION__ + std::string( ": ObjectMagicActionAck indicates error: " ) + Error( msg.error_code() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Error
// Created: MPT 2009-12-22
// -----------------------------------------------------------------------------
std::string DatabaseUpdater::Error( const MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode& error_code ) const
{
    switch ( error_code )
    {
    case MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_no_error:
        return "no_error";
    case MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_camp:
        return "error_invalid_camp";
    case MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_id:
        return "error_invalid_id";
    case MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_localisation:
        return "error_invalid_location";
    case MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_object:
        return "error_invalid_object";
    case MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode_error_invalid_specific_attributes:
        return "error_invalid_specific_attributes";
    default:
        return "unknown error code";
    }

}