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
void QueryDatabaseUpdater::Update( const ASN1T_MsgUnitCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "UnitForces" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid );
    builder.SetField( "parent_oid", msg.oid_automate );
    builder.SetField( "name", std::string( msg.nom ) );
    builder.SetField( "type", msg.type_pion );
    builder.SetField( "session_id", session_.GetId() );
    UpdateSymbol( builder, model_.agents_, msg.oid );    
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
void QueryDatabaseUpdater::Update( const ASN1T_MsgUnitKnowledgeCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "KnowledgeUnits" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid );
    builder.SetField( "group_oid", msg.oid_groupe_possesseur );
    builder.SetField( "unit_oid", msg.oid_unite_reelle );
    // $$$$ NEEDED ? builder.SetField( "type", msg.type_unite );
	builder.SetField( "session_id", session_.GetId() );

    if( const dispatcher::Agent* realAgent = model_.agents_.Find( msg.oid_unite_reelle ) )
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
void QueryDatabaseUpdater::Update( const ASN1T_MsgObjectKnowledgeCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "KnowledgeObjects" ) );

	const dispatcher::ObjectKnowledge* knowledge = model_.objectKnowledges_.Find( msg.oid );
	std::string symbol = knowledge->pObject_->GetType().GetSymbol(); 

	builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid );
	builder.SetField( "type", msg.type );
	builder.SetField( "team_id", msg.team );

	if ( msg.attributes.m.constructionPresent )
		builder.SetField( "state", msg.attributes.construction.percentage );

	if ( tools::app6::GetAffiliation( symbol ) != "U" )
		builder.SetField( "name",  std::string(knowledge->pObject_->GetName() ) );
	builder.SetField( "session_id", session_.GetId() );
	
	if( const dispatcher::KnowledgeGroup* knowledgeGroup = model_.knowledgeGroups_.Find( msg.team ) )
	{
		tools::app6::SetAffiliation( symbol, (unsigned int) knowledgeGroup->team_.GetKarma().GetUId() );
        builder.SetField( "observer_affiliation", tools::app6::GetAffiliation( symbol ) );
	}
	else
		tools::app6::SetAffiliation( symbol, (unsigned int) ASN1T_EnumDiplomacy::inconnu );

	builder.SetField( "symbol_id", FormatSymbol( symbol ) );

    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const ASN1T_MsgLimitCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "BoundaryLimits" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid );
    builder.SetField( "name", std::string( msg.tactical_line.name ) );
    builder.SetField( "symbol_id", std::string( "G-GPGLB----H--X" ) );
	builder.SetField( "session_id", session_.GetId() );
    builder.SetGeometry( Line( msg.tactical_line.geometry.coordinates ) );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const ASN1T_MsgLimaCreation& msg )
{    
    InsertQueryBuilder builder( database_.GetTableName( "BoundaryLimits" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid );
    builder.SetField( "name", std::string( msg.tactical_line.name ) );
	builder.SetField( "session_id", session_.GetId() );
    builder.SetGeometry( Line( msg.tactical_line.geometry.coordinates ) );
    database_.Execute( builder );
}

namespace
{
    std::string GetObjectTable( const ASN1T_Location& location )
    {
        std::string className = "TacticalObject_Area";
        switch ( location.type )
        {
        case EnumLocationType::point: className = "TacticalObject_Point"; break;
        case EnumLocationType::line:  className = "TacticalObject_Line"; break;
        }
        return className;
    }

	std::string GetObjectKnowledgeTable( const ASN1T_Location& location )
    {
        std::string className = "KnowledgeObjects_Area";
        switch ( location.type )
        {
        case EnumLocationType::point: className = "KnowledgeObjects_Point"; break;
        case EnumLocationType::line:  className = "KnowledgeObjects_Line"; break;
        }
        return className;
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::UpdateGeometry
// Created: JCR 2009-11-02
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::UpdateGeometry( QueryBuilder_ABC& builder, const ASN1T_Location& location )
{
    switch ( location.type )
    {
    case EnumLocationType::point:    builder.SetGeometry( Point( location.coordinates.elem[ 0 ] ) ); break;
    case EnumLocationType::line:     builder.SetGeometry( Line( location.coordinates ) ); break;
	case EnumLocationType::polygon:  builder.SetGeometry( Area( location.coordinates ) ); break;
    default:                      /*row.SetShape( Location( asn.location ) );*/ break; // $$$$ SBO 2007-08-31: TODO
    }
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// Modified: MPT 2009-09-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const ASN1T_MsgObjectKnowledgeUpdate& msg )
{
	UpdateQueryBuilder builder( database_.GetTableName( "KnowledgeObjects" ) );
	std::stringstream query;
    query << "public_oid=" << msg.oid << " AND session_id=" << session_.GetId();
	builder.SetClause( query.str() );
	builder.SetField( "team_id", msg.team );

	if ( msg.m.attributesPresent && msg.attributes.m.constructionPresent )
		builder.SetField( "state", msg.attributes.construction.percentage );

	const dispatcher::ObjectKnowledge* knowledge = model_.objectKnowledges_.Find( msg.oid );
	std::string symbol( knowledge->pObject_->GetType().GetSymbol() ); 
	if ( tools::app6::GetAffiliation( symbol ) != "U" )
		builder.SetField( "name",  std::string(knowledge->pObject_->GetName() ) );

	if( const dispatcher::KnowledgeGroup* knowledgeGroup = model_.knowledgeGroups_.Find( msg.team ) )
	{
		tools::app6::SetAffiliation( symbol, (unsigned int) knowledgeGroup->team_.GetKarma().GetUId() );
        builder.SetField( "observer_affiliation", tools::app6::GetAffiliation( symbol ) );
	}
	else
		tools::app6::SetAffiliation( symbol, (unsigned int) ASN1T_EnumDiplomacy::inconnu );

	builder.SetField( "symbol_id", FormatSymbol( symbol ) );
	database_.Execute( builder );
	
	//Geometrie
	if ( msg.m.locationPresent )
	    UpdateObjectKnowledgeGeometry( GetObjectKnowledgeTable( msg.location ), msg );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::UpdateObjectKnowledgeGeometry
// Created: JCR 2009-11-02
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::UpdateObjectKnowledgeGeometry( const std::string& tablename, const ASN1T_MsgObjectKnowledgeUpdate& msg )
{
	std::auto_ptr< Table_ABC > table( database_.OpenTable( tablename ) );
	std::stringstream query;
	query << "public_oid=" << msg.oid << " AND session_id=" << session_.GetId();
	Row_ABC* row = table->Find( query.str() );
	bool bHasRows = row != 0;
	if ( ! bHasRows )
	{
		InsertQueryBuilder builderGeom( database_.GetTableName( tablename ) );
		builderGeom.SetField( "public_oid", msg.oid );
		builderGeom.SetField( "session_id", session_.GetId() );
		UpdateGeometry( builderGeom, msg.location );
		database_.Execute( builderGeom );
	}
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// Modified: MPT 2009-09-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const ASN1T_MsgObjectCreation& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( GetObjectTable( msg.location ) ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", msg.oid );
    builder.SetField( "name", std::string( msg.name ) );
    builder.SetField( "type", std::string( msg.type ) );
	builder.SetField( "session_id", session_.GetId() );
    UpdateSymbol( builder, model_.objects_, msg.oid );
    UpdateGeometry( builder, msg.location );
   
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

/*void QueryDatabaseUpdater::Update( const ASN1T_MsgObjectCreation& msg )
{
	std::auto_ptr< Table_ABC > table( database_.OpenTable( GetObjectTable( msg.location ) ) );
	Row_ABC& row = table->CreateRow();
	row.SetField( "public_oid", FieldVariant( (int)msg.oid ) );
	row.SetField( "name", FieldVariant( std::string( msg.name ) ) );
	row.SetField( "type", FieldVariant( std::string( msg.type ) ) );
	row.SetField( "session_id", FieldVariant( session_.GetId() ) );
	switch ( msg.location.type )
    {
	case EnumLocationType::point:    row.SetShape( Point( msg.location.coordinates.elem[ 0 ] ) ); break;
	case EnumLocationType::line:     row.SetShape( Line( msg.location.coordinates ) ); break;
	case EnumLocationType::polygon:  row.SetShape( Area( msg.location.coordinates ) ); break;
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
void QueryDatabaseUpdater::Update( const ASN1T_MsgReport& msg )
{
    InsertQueryBuilder builder( database_.GetTableName( "Reports" ) );
    
    builder.SetId( "id" );
    builder.SetField( "unit_id", msg.oid );
    builder.SetField( "message", reportFactory_.CreateMessage( msg ) );
	builder.SetField( "session_id", session_.GetId() );
    // builder.AddParameter( "timestamp", reportFactory_.CreateMessage( msg ) );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const ASN1T_MsgFormationCreation& asn )
{
    InsertQueryBuilder builder( database_.GetTableName( "Formations" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", asn.oid );
    if ( asn.m.oid_formation_parentePresent )
        builder.SetField( "parent_oid", asn.oid_formation_parente );
    else
        builder.SetField( "parent_oid", 0 );
    builder.SetField( "name", std::string( asn.nom ) );
    builder.SetField( "type", -1 );
    builder.SetField( "engaged", 0 );
	builder.SetField( "session_id", session_.GetId() );
    UpdateSymbol( builder, model_.formations_, asn.oid );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const ASN1T_MsgAutomatCreation& asn )
{
    InsertQueryBuilder builder( database_.GetTableName( "Formations" ) );
    
    builder.SetId( "id" );
    builder.SetField( "public_oid", asn.oid );
    if ( asn.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation )
        builder.SetField( "parent_oid", asn.oid_parent.u.formation );
    else
        builder.SetField( "parent_oid", asn.oid_parent.u.automate );
    builder.SetField( "name", std::string( asn.nom ) );
    builder.SetField( "type", asn.type_automate );
    builder.SetField( "engaged", 0 );
	builder.SetField( "session_id", session_.GetId() );
    UpdateSymbol( builder, model_.automats_, asn.oid );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: JCR 2008-07-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const ASN1T_MsgAutomatAttributes& msg )
{
    UpdateQueryBuilder builder( database_.GetTableName( "Formations" ) );    
    std::stringstream query;

    query << "public_oid=" << msg.oid << " AND session_id=" << session_.GetId();
    builder.SetClause( query.str() );
    if( msg.m.etat_automatePresent )
        builder.SetField( "engaged", ( msg.etat_automate == EnumAutomatMode::embraye ) ? -1 : 0 );        
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
void QueryDatabaseUpdater::Update( const ASN1T_MsgUnitAttributes& msg )
{    
    UpdateQueryBuilder builder( database_.GetTableName( "UnitForces" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid << " AND session_id=" << session_.GetId();
    
    builder.SetClause( query.str() );
	if( msg.m.vitessePresent )
        builder.SetField( "speed", msg.vitesse );
    if( msg.m.etat_operationnel_brutPresent )
        builder.SetField( "op_state", msg.etat_operationnel_brut );
    if( msg.m.directionPresent )
        builder.SetField( "direction", msg.direction );
    if( msg.m.positionPresent ) 
        builder.SetGeometry( Point( msg.position ) );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::Update( const ASN1T_MsgUnitKnowledgeUpdate& msg )
{
    UpdateQueryBuilder builder( database_.GetTableName( "KnowledgeUnits" ) );
    std::stringstream query;
    query << "public_oid=" << msg.oid << " AND session_id=" << session_.GetId();
    
	//AME 2009-10-12 TODO: update field observer_oid 
    builder.SetClause( query.str() );
    if( msg.m.speedPresent )
        builder.SetField( "speed", msg.speed );
    if( msg.m.directionPresent )
        builder.SetField( "direction", msg.speed );
    if( msg.m.identification_levelPresent )
        builder.SetField( "identification_level", msg.identification_level );
    if( msg.m.max_identification_levelPresent )
        UpdateSymbol( builder, model_.agentKnowledges_, msg.oid );
	if ( msg.m.mortPresent )
		builder.SetField( "dead", msg.mort );
    if( msg.m.positionPresent )
        builder.SetGeometry( Point( msg.position ) );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::DestroyUnit
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::DestroyUnit( const ASN1T_MsgUnitDestruction& msg )
{
    std::stringstream query;
    query << "public_oid=" << msg << " AND session_id=" << session_.GetId();
    DeleteQueryBuilder builder( database_.GetTableName( "UnitForces" ) );
    builder.SetClause( query.str() );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::DestroyObject( const ASN1T_MsgObjectDestruction& msg )
{
    std::stringstream query;
    query << "public_oid=" << msg << " AND session_id=" << session_.GetId();
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
void QueryDatabaseUpdater::DestroyObjectKnowledge( const ASN1T_MsgObjectKnowledgeDestruction& msg )
{
    std::stringstream query;
    
	query << "public_oid=" << msg.oid << " AND session_id=" << session_.GetId();
    DeleteQueryBuilder builder( database_.GetTableName( "KnowledgeObjects" ) );
    builder.SetClause( query.str() );
    database_.Execute( builder );
}

// -----------------------------------------------------------------------------
// Name: QueryDatabaseUpdater::DestroyUnitKnowledge
// Created: AME 2009-10-12
// -----------------------------------------------------------------------------
void QueryDatabaseUpdater::DestroyUnitKnowledge( const ASN1T_MsgUnitKnowledgeDestruction& msg )
{
	 std::stringstream query;
    
	query << "public_oid=" << msg.oid << " AND session_id=" << session_.GetId();
    DeleteQueryBuilder builder( database_.GetTableName( "Knowledgeunits" ) );
    builder.SetClause( query.str() );
    database_.Execute( builder );
}
