// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "QueryDatabaseUpdater.h"
#include "ObjectAttributeUpdater.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "ReportFactory.h"
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
// Name: QueryDatabaseUpdater constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
QueryDatabaseUpdater::QueryDatabaseUpdater( Database_ABC& database, const dispatcher::Model& model, const ReportFactory& reportFactory, const WorkingSession& session )
    : database_     ( database )
    , model_        ( model )
    , reportFactory_( reportFactory )
    , session_      ( session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
QueryDatabaseUpdater::~QueryDatabaseUpdater()
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
    void UpdateSymbol( QueryBuilder_ABC& builder, List& list, int id )
    {
        if( const kernel::Entity_ABC* element = list.Find( id ) )
            builder.SetField( "symbol_id", FormatSymbol( element->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Flush
// Created: JCR 2009-04-22
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Flush()
{
    database_.Flush();
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::CleanSession
// Created: MPT 2009-07-28
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Clean()
{
    try
    {
        const std::string clause( "session_id=" + boost::lexical_cast< std::string >( session_.GetId() ) );

        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "UnitForces" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "KnowledgeUnits" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "BoundaryLimits" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "TacticalLines" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "TacticalObject_Area" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "TacticalObject_Line" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "TacticalObject_Point" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "KnowledgeObjects" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "KnowledgeUnits" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "Reports" ), clause ) );
        database_.Execute( DeleteQueryBuilder( database_.GetTableName( "Formations" ), clause ) );  
    }
    catch ( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "QueryDatabaseUpdater is not correctly loaded : " + std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgUnitCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "UnitForces" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", ( long ) msg.oid() );
    builder.SetField( "parent_oid", ( long ) msg.oid_automate() );
    builder.SetField( "name", std::string( msg.nom() ) );
    builder.SetField( "type", ( long ) msg.type_pion() );
    builder.SetField( "session_id", session_.GetId() );
    UpdateSymbol( builder, model_.agents_, msg.oid() );    
    builder.SetGeometry( Point() );
    database_.Execute( builder );
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
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "KnowledgeUnits" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid() );
    builder.SetField( "group_oid", msg.oid_groupe_possesseur() );
    builder.SetField( "unit_oid", msg.oid_unite_reelle() );
    // $$$$ NEEDED ? builder.SetField( "type", msg.type_unite );
    builder.SetField( "session_id", session_.GetId() );

    if( const dispatcher::Agent* realAgent = model_.agents_.Find( msg.oid_unite_reelle() ) )
    {
        std::string a = tools::app6::GetAffiliation( realAgent->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() );
        builder.SetField( "observer_affiliation", InverseAffiliation( a ) );
    }
    builder.SetField( "symbol_id", FormatSymbol( "SUZP********---" ) ); // $$$$ SBO 2007-09-27: hard coded...
    builder.SetGeometry( Point() );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: AME 2009-10-12
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "KnowledgeObjects" ) );

    const dispatcher::ObjectKnowledge* knowledge = model_.objectKnowledges_.Find( msg.oid() );
    std::string symbol = knowledge->pObject_->GetType().GetSymbol(); 

    builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid() );
    builder.SetField( "type", msg.type() );
    builder.SetField( "team_id", msg.team() );

    if ( msg.attributes().has_construction() )
        builder.SetField( "state", msg.attributes().construction().percentage() );

    if ( tools::app6::GetAffiliation( symbol ) != "U" )
        builder.SetField( "name",  std::string(knowledge->pObject_->GetName() ) );
    builder.SetField( "session_id", session_.GetId() );
    
    if( const dispatcher::KnowledgeGroup* knowledgeGroup = model_.knowledgeGroups_.Find( msg.team() ) )
    {
        tools::app6::SetAffiliation( symbol, (unsigned int) knowledgeGroup->GetTeam().GetKarma().GetUId() );
        builder.SetField( "observer_affiliation", tools::app6::GetAffiliation( symbol ) );
    }
    else
        tools::app6::SetAffiliation( symbol, (unsigned int) Common::EnumDiplomacy::unknown_diplo );

    builder.SetField( "symbol_id", FormatSymbol( symbol ) );

    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsMessengerToClient::MsgLimitCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "BoundaryLimits" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", ( long ) msg.oid() );
    builder.SetField( "name", std::string( msg.tactical_line().name() ) );
    builder.SetField( "symbol_id", std::string( "G-GPGLB----H--X" ) );
    builder.SetField( "session_id", session_.GetId() );
    builder.SetGeometry( Line( msg.tactical_line().geometry().coordinates() ) );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsMessengerToClient::MsgLimaCreation& msg )
{    
    InsertQueryBuilder builder( database_.GetTableName( "BoundaryLimits" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", ( long ) msg.oid() );
    builder.SetField( "name", std::string( msg.tactical_line().name() ) );
    builder.SetField( "session_id", session_.GetId() );
    builder.SetGeometry( Line( msg.tactical_line().geometry().coordinates() ) );
    database_.Execute( builder );
}

namespace
{
    std::string GetObjectTable( const Common::MsgLocation& location )
    {
        std::string className = "TacticalObject_Area";
        switch ( location.type() )
        {
        case Common::MsgLocation_Geometry_point: className = "TacticalObject_Point"; break;
        case Common::MsgLocation_Geometry_line:  className = "TacticalObject_Line"; break;
        }
        return className;
    }

    std::string GetObjectKnowledgeTable( const Common::MsgLocation& location )
    {
        std::string className = "KnowledgeObjects_Area";
        switch ( location.type() )
        {
        case Common::MsgLocation_Geometry_point: className = "KnowledgeObjects_Point"; break;
        case Common::MsgLocation_Geometry_line:  className = "KnowledgeObjects_Line"; break;
        }
        return className;
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::UpdateGeometry
// Created: JCR 2009-11-02
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::UpdateGeometry( QueryBuilder_ABC& builder, const Common::MsgLocation& location )
{
    switch ( location.type() )
    {
    case Common::MsgLocation_Geometry_point:    builder.SetGeometry( Point( location.coordinates().elem( 0 ) ) ); break;
    case Common::MsgLocation_Geometry_line:     builder.SetGeometry( Line( location.coordinates() ) ); break;
    case Common::MsgLocation_Geometry_polygon:  builder.SetGeometry( Area( location.coordinates() ) ); break;
    default:    break;
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// Modified: MPT 2009-09-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg )
{
    UpdateQueryBuilder builder( database_.GetTableName( "KnowledgeObjects" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    builder.SetClause( query.str() );
    builder.SetField( "team_id", msg.team() );

    if ( msg.has_attributes() && msg.attributes().has_construction() )
        builder.SetField( "state", msg.attributes().construction().percentage() );

    const dispatcher::ObjectKnowledge* knowledge = model_.objectKnowledges_.Find( msg.oid() );
    std::string symbol( knowledge->pObject_->GetType().GetSymbol() ); 
    if ( tools::app6::GetAffiliation( symbol ) != "U" )
        builder.SetField( "name",  std::string(knowledge->pObject_->GetName() ) );

    if( const dispatcher::KnowledgeGroup* knowledgeGroup = model_.knowledgeGroups_.Find( msg.team() ) )
    {
        tools::app6::SetAffiliation( symbol, (unsigned int) knowledgeGroup->GetTeam().GetKarma().GetUId() );
        builder.SetField( "observer_affiliation", tools::app6::GetAffiliation( symbol ) );
    }
    else
        tools::app6::SetAffiliation( symbol, (unsigned int) Common::EnumDiplomacy::unknown_diplo );

    builder.SetField( "symbol_id", FormatSymbol( symbol ) );
    database_.Execute( builder );
    
    //Geometrie
    if ( msg.has_location() )
        UpdateObjectKnowledgeGeometry( GetObjectKnowledgeTable( msg.location() ), msg );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::UpdateObjectKnowledgeGeometry
// Created: JCR 2009-11-02
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::UpdateObjectKnowledgeGeometry( const std::string& tablename, const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    Row_ABC* row = table->Find( query.str() );
    bool bHasRows = row != 0;
    if ( ! bHasRows )
    {
        InsertQueryBuilder builderGeom( database_.GetTableName( tablename ) );
        builderGeom.SetField( "public_oid", msg.oid() );
        builderGeom.SetField( "session_id", session_.GetId() );
        UpdateGeometry( builderGeom, msg.location() );
        database_.Execute( builderGeom );
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// Modified: MPT 2009-09-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgObjectCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( GetObjectTable( msg.location() ) ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid() );
    builder.SetField( "name", std::string( msg.name() ) );
    builder.SetField( "type", std::string( msg.type() ) );
    builder.SetField( "session_id", session_.GetId() );
    UpdateSymbol( builder, model_.objects_, msg.oid() );
    UpdateGeometry( builder, msg.location() );
   
    database_.Execute( builder );

    // retrieve object id for dumping attributes
    /*std::stringstream query;
    query << "public_oid=" << msg.oid ;
    std::auto_ptr< Table_ABC > table( database_.OpenTable( GetObjectTable( msg.location ) ) );
    Row_ABC* pRow = table->Find( query.str(), true );
    FieldVariant res = pRow->GetField( "id" );
    int objectId = boost::get<int>(res);
    ObjectAttributeUpdater::UpdateObjectAttribute( database_, objectId, msg.attributes );*/
}
// $$$$ _RC_ FDS 2010-01-21: To Remove ???
/*void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgObjectCreation& msg )
{
    std::auto_ptr< Table_ABC > table( database_.OpenTable( GetObjectTable( msg.location ) ) );
    Row_ABC& row = table->CreateRow();
    row.SetField( "public_oid", FieldVariant( (int)msg.oid ) );
    row.SetField( "name", FieldVariant( std::string( msg.name ) ) );
    row.SetField( "type", FieldVariant( std::string( msg.type ) ) );
    row.SetField( "session_id", FieldVariant( session_.GetId() ) );
    switch ( msg.location.type )
    {
    case Geometry::point:    row.SetShape( Point( msg.location.coordinates.elem[ 0 ] ) ); break;
    case Geometry::line:     row.SetShape( Line( msg.location.coordinates ) ); break;
    case Geometry::polygon:  row.SetShape( Area( msg.location.coordinates ) ); break;
    }

    table->InsertRow( row );
    long objectId = row.GetID();

    // handle attributes
    ObjectAttributeUpdater::UpdateObjectAttribute( database_, objectId, msg.attributes );
}*/


// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgReport& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "Reports" ) );
    
    builder.SetId( "id" );
    builder.SetField( "unit_id", msg.oid() );
    builder.SetField( "message", reportFactory_.CreateMessage( msg ) );
    builder.SetField( "session_id", session_.GetId() );
    // builder.AddParameter( "timestamp", reportFactory_.CreateMessage( msg ) );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const Common::MsgFormationCreation& message )
{
    InsertQueryBuilder builder( database_.GetTableName( "Formations" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", message.oid() );
    if ( message.has_oid_formation_parente() )
        builder.SetField( "parent_oid", message.oid_formation_parente() );
    else
        builder.SetField( "parent_oid", 0 );
    builder.SetField( "name", std::string( message.nom() ) );
    builder.SetField( "type", -1 );
    builder.SetField( "engaged", 0 );
    builder.SetField( "session_id", session_.GetId() );
    UpdateSymbol( builder, model_.formations_, message.oid() );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgAutomatCreation& message )
{
    InsertQueryBuilder builder( database_.GetTableName( "Formations" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", ( long ) message.oid() );
    if ( message.oid_parent().has_formation() )
        builder.SetField( "parent_oid", message.oid_parent().formation().oid() );
    else
        builder.SetField( "parent_oid", message.oid_parent().automate().oid() );
    builder.SetField( "name", std::string( message.nom() ) );
    builder.SetField( "type", ( long ) message.type_automate() );
    builder.SetField( "engaged", 0 );
    builder.SetField( "session_id", session_.GetId() );
    UpdateSymbol( builder, model_.automats_, message.oid() );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: JCR 2008-07-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgAutomatAttributes& msg )
{
    UpdateQueryBuilder builder( database_.GetTableName( "Formations" ) );    
    std::stringstream query;

    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    builder.SetClause( query.str() );
    if( msg.has_etat_automate() )
        builder.SetField( "engaged", ( msg.etat_automate() == Common::EnumAutomatMode::embraye ) ? -1 : 0 );        
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
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
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgUnitAttributes& msg )
{    
    UpdateQueryBuilder builder( database_.GetTableName( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    
    builder.SetClause( query.str() );
    if( msg.has_vitesse() )
        builder.SetField( "speed", msg.vitesse() );
    if( msg.has_etat_operationnel_brut() )
        builder.SetField( "op_state", msg.etat_operationnel_brut() );
    if( msg.has_direction() )
        builder.SetField( "direction", msg.direction().heading() );
    if( msg.has_position() ) 
        builder.SetGeometry( Point( msg.position() ) );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg )
{
    UpdateQueryBuilder builder( database_.GetTableName( "KnowledgeUnits" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    
    //AME 2009-10-12 TODO: update field observer_oid 
    builder.SetClause( query.str() );
    if( msg.has_speed() )
        builder.SetField( "speed", msg.speed() );
    if( msg.has_direction() )
        builder.SetField( "direction", msg.speed() );
    if( msg.has_identification_level() )
        builder.SetField( "identification_level", msg.identification_level() );
    if( msg.has_max_identification_level() )
        UpdateSymbol( builder, model_.agentKnowledges_, msg.oid() );
    if ( msg.has_mort() )
        builder.SetField( "dead", msg.mort() );
    if( msg.has_position() )
        builder.SetGeometry( Point( msg.position() ) );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::DestroyUnit
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::DestroyUnit( const MsgsSimToClient::MsgUnitDestruction& msg )
{
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    DeleteQueryBuilder builder( database_.GetTableName( "UnitForces" ) );
    builder.SetClause( query.str() );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::DestroyObject( const MsgsSimToClient::MsgObjectDestruction& msg )
{
    std::stringstream query;
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    {
        DeleteQueryBuilder builder( database_.GetTableName( "TacticalObject_Point" ) );
        builder.SetClause( query.str() );
        database_.Execute( builder );
    }
    {
        DeleteQueryBuilder builder( database_.GetTableName( "TacticalObject_Line" ) );
        builder.SetClause( query.str() );
        database_.Execute( builder );
    }
    {
        DeleteQueryBuilder builder( database_.GetTableName( "TacticalObject_Area" ) );
        builder.SetClause( query.str() );
        database_.Execute( builder );
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::DestroyObjectKnowledge( const MsgsSimToClient::MsgObjectKnowledgeDestruction& msg )
{
    std::stringstream query;
    
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    DeleteQueryBuilder builder( database_.GetTableName( "KnowledgeObjects" ) );
    builder.SetClause( query.str() );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::DestroyUnitKnowledge
// Created: AME 2009-10-12
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::DestroyUnitKnowledge( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg )
{
     std::stringstream query;
    
    query << "public_oid=" << msg.oid() << " AND session_id=" << session_.GetId();
    DeleteQueryBuilder builder( database_.GetTableName( "Knowledgeunits" ) );
    builder.SetClause( query.str() );
    database_.Execute( builder );
}
