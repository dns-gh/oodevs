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
#include "dispatcher/Model.h"
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
#include "WorkingSession.h"

#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabaseUpdater::DatabaseUpdater( Workspace_ABC& workspace, const dispatcher::Model& model, const WorkingSession& session )
    : geometryDb_ ( workspace.GetDatabase( "geometry" ) )
    , flatDb_   ( workspace.GetDatabase( "flat" ) )
    , model_    ( model )
    , session_  ( session )
{
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
void DatabaseUpdater::Flush()
{
    geometryDb_.Flush();
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

        geometryDb_.ClearTable( "UnitForces", clause );
        geometryDb_.ClearTable( "KnowledgeUnits", clause );
        geometryDb_.ClearTable( "BoundaryLimits", clause );
        geometryDb_.ClearTable( "TacticalLines", clause );
        geometryDb_.ClearTable( "TacticalObject_Area", clause );
        geometryDb_.ClearTable( "TacticalObject_Line", clause );
        geometryDb_.ClearTable( "TacticalObject_Point", clause );
        geometryDb_.ClearTable( "KnowledgeObjects", clause );
        geometryDb_.ClearTable( "KnowledgeUnits", clause );
        geometryDb_.ClearTable( "Reports", clause );
        flatDb_.ClearTable( "Formations", clause );
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
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "UnitForces" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.oid() ) );
    row.SetField( "parent_oid", FieldVariant( ( long ) msg.oid_automate() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.nom() ) ) );
    row.SetField( "type"      , FieldVariant( ( long ) msg.type_pion() ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Agents(), msg.oid() );
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
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "KnowledgeUnits" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid"  , FieldVariant( ( long ) msg.oid() ) );
    row.SetField( "group_oid"   , FieldVariant( ( long ) msg.oid_groupe_possesseur() ) );
    row.SetField( "unit_oid", FieldVariant( ( long ) msg.oid_unite_reelle() ) );
   // $$$$ NEEDED ? builder.SetField( "type", msg.type_unite );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );

    if( const dispatcher::Agent_ABC* realAgent = model_.Agents().Find( msg.oid_unite_reelle() ) )
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
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "KnowledgeObjects" ) );

    const dispatcher::ObjectKnowledge* knowledge = model_.objectKnowledges_.Find( msg.oid() );
    const kernel::Object_ABC* entity = knowledge->GetEntity();
    if( !entity ) // $$$$ _RC_ SBO 2010-06-10: no real object => giving up
        return;
    std::string symbol = entity->GetType().GetSymbol();
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( (long) msg.oid() ) );
    row.SetField( "type", FieldVariant( std::string( msg.type() ) ) );
    row.SetField( "team_id", FieldVariant( msg.team() ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );

    if( msg.attributes().has_construction() )
        row.SetField( "state", FieldVariant( msg.attributes().construction().percentage() ) );
    if( tools::app6::GetAffiliation( symbol ) != "U" )
        row.SetField( "name", FieldVariant( std::string( entity->GetName().ascii() ) ) );
    if( const dispatcher::KnowledgeGroup* knowledgeGroup = static_cast< const dispatcher::KnowledgeGroup* >( model_.KnowledgeGroups().Find( msg.team() ) ) )
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
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "BoundaryLimits" ) );

    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.oid() ) );
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
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "TacticalLines" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( (long) msg.oid() ) );
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
    void UpdateGeometry( Row_ABC& row, const Common::MsgLocation& location )
    {
        switch ( location.type() )
        {
        case Common::MsgLocation::point:    row.SetGeometry( Point( location.coordinates().elem( 0 ) ) ); break;
        case Common::MsgLocation::line:     row.SetGeometry( Line( location.coordinates() ) ); break;
        case Common::MsgLocation::polygon:  row.SetGeometry( Area( location.coordinates() ) ); break;
        default:                      /*row.SetGeometry( Location( asn.location ) );*/ break; // $$$$ SBO 2007-08-31: TODO
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectCreation& msg )
{
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( GetObjectTable( msg.location() ) ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( (long) msg.oid() ) );
    row.SetField( "name" , FieldVariant( std::string( msg.name() ) ) );
    row.SetField( "type" , FieldVariant( std::string( msg.type() ) ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Objects(), msg.oid() );
    UpdateGeometry( row, msg.location() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// Modified: MPT 2009-09-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "KnowledgeObjects" ) );

    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();

    if( Row_ABC* row = table->Find( query.str() ) )
    {
        row->SetField( "team_id", FieldVariant( msg.team() ) );
        if( msg.has_attributes() && msg.attributes().has_construction() )
            row->SetField( "state", FieldVariant( msg.attributes().construction().percentage() ) );

        const dispatcher::ObjectKnowledge* knowledge = model_.objectKnowledges_.Find( msg.oid() );
        const kernel::Object_ABC* entity = knowledge->GetEntity();
        if( !entity ) // $$$$ _RC_ SBO 2010-06-10: no real object => giving up
            return;
        std::string symbol = entity->GetType().GetSymbol();
        if( tools::app6::GetAffiliation( symbol ) != "U" )
            row->SetField( "name",  FieldVariant( std::string( entity->GetName() ) ) );

        if( const dispatcher::KnowledgeGroup* knowledgeGroup = static_cast< const dispatcher::KnowledgeGroup* >( model_.KnowledgeGroups().Find( msg.team() ) ) )
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
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( tablename ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();

    Row_ABC* row = table->Find( query.str() );
    if( row == NULL )
    {
        row = &table->CreateRow();
        row->SetField( "public_oid", FieldVariant( (long) msg.oid() ) );
        row->SetField( "session_id", FieldVariant( session_.GetId() ) );
    }
    UpdateGeometry( *row, msg.location() );
}



// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const Common::MsgFormationCreation& message )
{
    std::auto_ptr< Table_ABC > table( flatDb_.OpenTable( "Formations" ) );

    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) message.oid() ) );
    if( message.has_oid_formation_parente() )
        row.SetField( "parent_oid", FieldVariant( ( long ) message.oid_formation_parente() ) );
    else
        row.SetField( "parent_oid", FieldVariant( ( long ) 0 ) );
    row.SetField( "name", FieldVariant( std::string( message.nom() ) ) );
    row.SetField( "type", FieldVariant( -1 ) );
    row.SetField( "engaged", FieldVariant( 0 ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Formations(), message.oid() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgAutomatCreation& message )
{
    std::auto_ptr< Table_ABC > table( flatDb_.OpenTable( "Formations" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) message.oid() ) );
    if( message.oid_parent().has_formation() )
        row.SetField( "parent_oid", FieldVariant( ( long ) message.oid_parent().formation().oid() ) );
    else
        row.SetField( "parent_oid", FieldVariant( ( long ) message.oid_parent().automate().oid() ) );
    row.SetField( "name", FieldVariant( std::string( message.nom() ) ) );
    row.SetField( "type", FieldVariant( ( long ) message.type_automate() ) );
    row.SetField( "engaged", FieldVariant( true ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    UpdateSymbol( row, model_.Automats(), ( long ) message.oid() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: JCR 2008-07-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgAutomatAttributes& msg )
{
    std::auto_ptr< Table_ABC > table( flatDb_.OpenTable( "Formations" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();

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
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();

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
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "KnowledgeUnits" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();

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
            UpdateSymbol( *row, model_.AgentKnowledges(), msg.oid() );
        if( msg.has_position() )
            row->SetGeometry( Point( msg.position() ) );
        table->UpdateRow( *row );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyUnit
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::DestroyUnit( const MsgsSimToClient::MsgUnitDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    table->DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabaseUpdater::DestroyObject( const MsgsSimToClient::MsgObjectDestruction& msg )
{
    std::stringstream ssQuery;
    ssQuery << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    std::string query( ssQuery.str() );
    {
        std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "TacticalObject_Point" ) );
        table->DeleteRows( query );
    }
    {
        std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "TacticalObject_Line" ) );
        table->DeleteRows( query );
    }
    {
        std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "TacticalObject_Area" ) );
        table->DeleteRows( query );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::DestroyUnitKnowledge( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "KnowledgeUnits" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    table->DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabaseUpdater::DestroyObjectKnowledge( const MsgsSimToClient::MsgObjectKnowledgeDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( geometryDb_.OpenTable( "KnowledgeObjects" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    table->DeleteRows( query.str() );
}
