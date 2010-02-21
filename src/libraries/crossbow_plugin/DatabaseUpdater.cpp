// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "DatabaseUpdater.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "ReportFactory.h"
#include "Line.h"
#include "Point.h"
#include "dispatcher/EntitySymbols_ABC.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Object.h"
#include "dispatcher/AgentKnowledge.h"
#include "tools/App6Symbol.h"
#include "QueryBuilder.h"

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabaseUpdater::DatabaseUpdater( Database_ABC& database, const dispatcher::Model& model, const ReportFactory& reportFactory )
    : database_     ( database )
    , model_        ( model )
    , reportFactory_( reportFactory )
{
    try 
    {
        database.ClearTable( "UnitForces" );
        database.ClearTable( "KnowledgeUnits" );
        database.ClearTable( "BoundaryLimits" );
        database.ClearTable( "TacticalLines" );
        database.ClearTable( "TacticalObject_Area" );
        database.ClearTable( "TacticalObject_Line" );
        database.ClearTable( "TacticalObject_Point" );
        database.ClearTable( "Reports" );
        database.ClearTable( "Formations" );        
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "QueryDatabaseUpdater is not correctly loaded : " + std::string( e.what() ) );
    }
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
        if ( result.size() < 15 )        
            result.append( std::string( 15 - symbol.size(), '-' ) );        
        return result;
    }

    template< typename List >
    void UpdateSymbol( Row_ABC& row, List& list, int id )
    {
        if( const kernel::Entity_ABC* element = list.Find( id ) )
            row.SetField( "symbol_id", FieldVariant( FormatSymbol( element->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() ) ) );
    }

    template< typename List >
    void UpdateSymbol( QueryBuilder_ABC& builder, List& list, int id )
    {
        if( const kernel::Entity_ABC* element = list.Find( id ) )
            builder.SetField( "symbol_id", FormatSymbol( element->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Flush
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void DatabaseUpdater::Flush()
{
    database_.Flush();
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "UnitForces" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.oid() ) );
    row.SetField( "parent_oid", FieldVariant( ( long ) msg.oid_automate() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.nom() ) ) );
    row.SetField( "type"      , FieldVariant( ( long ) msg.type_pion() ) );
    UpdateSymbol( row, model_.agents_, msg.oid() );
    row.SetShape( Point() );
    table->InsertRow( row );
}

namespace 
{
    std::string InverseAffiliation( const std::string& affiliation )
    {
        if ( affiliation == "H" ) return "F";
        if ( affiliation == "F" ) return "H";
        return affiliation;
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "KnowledgeUnits" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid"  , FieldVariant( ( long ) msg.oid() ) );
    row.SetField( "group_oid"   , FieldVariant( ( long ) msg.oid_groupe_possesseur() ) );
    row.SetField( "unit_oid", FieldVariant( ( long ) msg.oid_unite_reelle() ) );

    if( const dispatcher::Agent* realAgent = model_.agents_.Find( msg.oid_unite_reelle() ) )
    {
        std::string a = tools::app6::GetAffiliation( realAgent->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() );
        row.SetField( "observer_affiliation", FieldVariant( InverseAffiliation( a ) ) );
    }
    row.SetField( "symbol_id", FieldVariant( FormatSymbol( "SUZP********---" ) ) ); // $$$$ SBO 2007-09-27: hard coded...
    row.SetShape( Point() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsMessengerToClient::MsgLimitCreation& msg )
{
//    QueryBuilder builder( "BoundaryLimits" );
//    builder.AddParameter( "public_oid", msg.oid );
//    builder.AddParameter( "name", std::string( msg.tactical_line.name ) );
//    builder.AddParameter( "symbol_id", std::string( "G-GPGLB----H--X" ) );
//    builder.SetGeometry( Line( msg.tactical_line.geometry.coordinates ) );
//    database_.Execute( builder );

    std::auto_ptr< Table_ABC > table( database_.OpenTable( "BoundaryLimits" ) );
   
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.oid() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.tactical_line().name() ) ) );
    row.SetField( "symbol_id" , FieldVariant( std::string( "G-GPGLB----H--X" ) ) ); // default
    row.SetShape( Line( msg.tactical_line().geometry().coordinates() ) );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsMessengerToClient::MsgLimaCreation& msg )
{    
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "TacticalLines" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.oid() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.tactical_line().name() ) ) );
    row.SetShape( Line( msg.tactical_line().geometry().coordinates() ) );
    table->InsertRow( row );
}

namespace
{
    std::string GetObjectTable( const Common::MsgLocation& location )
    {
        std::string className = "TacticalObject_Area";
        switch ( location.type() )
        {
        case MsgLocation::point: className = "TacticalObject_Point"; break;
        case MsgLocation::line:  className = "TacticalObject_Line"; break;
        }
        return className;
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgObjectCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( GetObjectTable( msg.location() ) ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) msg.oid() ) );
    row.SetField( "name"      , FieldVariant( std::string( msg.name() ) ) );
    UpdateSymbol( row, model_.objects_, msg.oid() );
    switch ( msg.location().type() )
    {
        case MsgLocation_Geometry_point: row.SetShape( Point( msg.location().coordinates().elem( 0 ) ) ); break;
        case MsgLocation_Geometry_line:  row.SetShape( Line( msg.location().coordinates() ) ); break;
        default:    break;
    }
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgReport& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "Reports" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "unit_id", FieldVariant( ( long ) msg.oid() ) );
    row.SetField( "message", FieldVariant( reportFactory_.CreateMessage( msg ) ) );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const Common::MsgFormationCreation& message )
{
    Table_ABC& table = database_.OpenBufferedTable( "Formations" );
    Row_ABC& row = table.CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) message.oid() ) );
    if( message.has_oid_formation_parente() )
        row.SetField( "parent_oid", FieldVariant( ( long ) message.oid_formation_parente() ) );
    else
        row.SetField( "parent_oid", FieldVariant( ( long ) 0 ) );
    row.SetField( "name", FieldVariant( std::string( message.nom() ) ) );
    row.SetField( "type", FieldVariant( -1 ) );
    row.SetField( "engaged", FieldVariant( 0 ) );
    UpdateSymbol( row, model_.formations_, message.oid() );
    table.InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgAutomatCreation& message )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "Formations" ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( ( long ) message.oid() ) );
    if( message.oid_parent().has_formation() )
        row.SetField( "parent_oid", FieldVariant( ( long ) message.oid_parent().formation().oid() ) );
    else
        row.SetField( "parent_oid", FieldVariant( ( long ) message.oid_parent().automate().oid() ) );
    row.SetField( "name", FieldVariant( std::string( message.nom() ) ) );
    row.SetField( "type", FieldVariant( ( long ) message.type_automate() ) );
    UpdateSymbol( row, model_.automats_, ( long ) message.oid() );
    table->InsertRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: JCR 2008-07-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgAutomatAttributes& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "Formations" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid();
    
    table->EndTransaction();
    if( Row_ABC* row = table->Find( query.str() ) )
    {
        if( msg.has_etat_automate() )
            row->SetField( "engaged", FieldVariant( ( msg.etat_automate() == EnumAutomatMode::embraye ) ? -1 : 0 ) );        
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
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid();
    
    if( msg.has_position() )
        table->BeginTransaction();
    else
        table->EndTransaction();
    if( Row_ABC* row = table->Find( query.str() ) )
    {
		if( msg.has_vitesse() )
            row->SetField( "speed", FieldVariant( msg.vitesse() ) );
        if( msg.has_etat_operationnel_brut() )
            row->SetField( "op_state", FieldVariant( msg.etat_operationnel_brut() ) );
        if( msg.has_position() )
            row->SetShape( Point( msg.position() ) );

		/*table->BeginTransaction();
        table->UpdateRow( *row );
		table->EndTransaction();*/
    }    
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg )
{    
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "KnowledgeUnits" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid();
    
    if( msg.has_position() )
        table->BeginTransaction();
    else
        table->EndTransaction();
    if( Row_ABC* row = table->Find( query.str() ) )
    {
        if( msg.has_speed() )
            row->SetField( "speed", FieldVariant( msg.speed() ) );
        if( msg.has_identification_level() )
            row->SetField( "identification_level", FieldVariant( msg.identification_level() ) );
        if( msg.has_mort() )
            row->SetField( "dead", FieldVariant( msg.mort() ) );
        if( msg.has_max_identification_level() )
            UpdateSymbol( *row, model_.agentKnowledges_, msg.oid() );
        if( msg.has_position() )
            row->SetShape( Point( msg.position() ) );
        table->UpdateRow( *row );
    }    
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyUnit
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::DestroyUnit( const MsgsSimToClient::MsgUnitDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid();
    table->DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( "KnowledgeUnits" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid();
    table->DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabaseUpdater::DestroyObject( const MsgsSimToClient::MsgObjectDestruction& msg )
{
    std::stringstream ss;
    ss << "public_oid=" << msg.oid();
    {
        std::auto_ptr< Table_ABC > table( database_.OpenTable( "TacticalObject_Point" ) );
        table->DeleteRows( ss.str() );
    }
    {
        std::auto_ptr< Table_ABC > table( database_.OpenTable( "TacticalObject_Line" ) );
        table->DeleteRows( ss.str() );
    }
    {
        std::auto_ptr< Table_ABC > table( database_.OpenTable( "TacticalObject_Area" ) );
        table->DeleteRows( ss.str() );
    }
}
