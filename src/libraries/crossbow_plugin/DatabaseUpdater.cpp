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
#include "ScopeEditor.h"
#include "ReportFactory.h"
#include "Line.h"
#include "Point.h"
#include "dispatcher/EntitySymbols_ABC.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Lima.h"
#include "dispatcher/Limit.h"
#include "dispatcher/Object.h"
#include "dispatcher/AgentKnowledge.h"
#include "tools/App6Symbol.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabaseUpdater::DatabaseUpdater( Database_ABC& database, const dispatcher::Model& model, const ReportFactory& reportFactory )
    : database_     ( database )
    , model_        ( model )
    , reportFactory_( reportFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
DatabaseUpdater::~DatabaseUpdater()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Lock
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabaseUpdater::Lock()
{
    database_.Lock();
}
    
// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Unlock
// Created: JCR 2008-01-11
// -----------------------------------------------------------------------------
void DatabaseUpdater::UnLock()
{
    database_.UnLock();
}

namespace
{
    std::string FormatSymbol( const std::string& symbol )
    {
        std::string result( symbol.size(), '*' );
        std::transform( symbol.begin(), symbol.end(), result.begin(), toupper );
        std::replace( result.begin(), result.end(), '*', '-' );
        return result;
    }

    template< typename List >
    void UpdateSymbol( Row_ABC& row, List& list, int id )
    {
        if( const dispatcher::Entity_ABC* element = list.Find( id ) )
            row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( element->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgUnitCreation& msg )
{
    ScopeEditor editor( database_ );
    Table_ABC&  table = database_.OpenTable( "UnitForces" );
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( msg.oid ) );
    row.SetField( "Parent_OID", FieldVariant( msg.oid_automate ) );
    row.SetField( "Name"      , FieldVariant( std::string( msg.nom ) ) );
    UpdateSymbol( row, model_.GetAgents(), msg.oid );
    row.SetShape( Point() );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgUnitKnowledgeCreation& msg )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( "KnowledgeUnits" );    
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID"  , FieldVariant( msg.oid ) );
    row.SetField( "Group_OID"   , FieldVariant( msg.oid_groupe_possesseur ) );
    row.SetField( "RealUnit_OID", FieldVariant( msg.oid_unite_reelle ) );

    if( const dispatcher::Agent* realAgent = model_.GetAgents().Find( msg.oid_unite_reelle ) )
        row.SetField( "ObserverAffiliation", FieldVariant( tools::app6::GetAffiliation( realAgent->Get< dispatcher::EntitySymbols_ABC >().BuildSymbol() ) ) );
    row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( "SUZP********---" ) ) ); // $$$$ SBO 2007-09-27: hard coded...
    row.SetShape( Point() );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgLimitCreation& msg )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( "BoundaryLimits" );
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( msg.oid ) );
    row.SetField( "Name"      , FieldVariant( std::string( msg.tactical_line.name ) ) );
    UpdateSymbol( row, model_.GetLimits(), msg.oid );
    row.SetShape( Line( msg.tactical_line.geometry.coordinates ) );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgLimaCreation& msg )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( "TacticalLines" );    
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( msg.oid ) );
    row.SetField( "Name"      , FieldVariant( std::string( msg.tactical_line.name ) ) );
    UpdateSymbol( row, model_.GetLimas(), msg.oid );
    row.SetShape( Line( msg.tactical_line.geometry.coordinates ) );
    table.UpdateRow( row );
}

namespace
{
    std::string GetObjectTable( const ASN1T_Location& location )
    {
        std::string className = "TacticalObjectArea";
        switch ( location.type )
        {
        case EnumLocationType::point: className = "TacticalObjectPoint"; break;
        case EnumLocationType::line:  className = "TacticalObjectLine"; break;
        }
        return className;
    }
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgObjectCreation& msg )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( GetObjectTable( msg.location ) );
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( msg.oid ) );
    row.SetField( "Name"      , FieldVariant( std::string( msg.name ) ) );
    UpdateSymbol( row, model_.GetObjects(), msg.oid );
    switch ( msg.location.type )
    {
    case EnumLocationType::point: row.SetShape( Point( msg.location.coordinates.elem[ 0 ] ) ); break;
    case EnumLocationType::line:  row.SetShape( Line( msg.location.coordinates ) ); break;
    default:                      /*row.SetShape( Location( asn.location ) );*/ break; // $$$$ SBO 2007-08-31: TODO
    }
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgReport& msg )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( "Reports" );
    Row_ABC& row = table.CreateRow();
    row.SetField( "unit_id", FieldVariant( msg.oid ) );
    row.SetField( "message", FieldVariant( reportFactory_.CreateMessage( msg ) ) );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgFormationCreation& asn )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( "Formations" );
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( asn.oid ) );
    row.SetField( "Parent_OID", FieldVariant( asn.oid_formation_parente ) );
    row.SetField( "Name"      , FieldVariant( std::string( asn.nom ) ) );
    UpdateSymbol( row, model_.GetFormations(), asn.oid );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgAutomatCreation& asn )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( "Formations" );
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( asn.oid ) );
    if( asn.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation )
        row.SetField( "Parent_OID", FieldVariant( asn.oid_parent.u.formation ) );
    else
        row.SetField( "Parent_OID", FieldVariant( asn.oid_parent.u.automate ) );
    row.SetField( "Name", FieldVariant( std::string( asn.nom ) ) );
    UpdateSymbol( row, model_.GetAutomats(), asn.oid );
    table.UpdateRow( row );
}

namespace 
{
    class ScopeTransaction
    {
    public:
        explicit ScopeTransaction( Table_ABC& table ) 
            : table_ ( table ) 
        {
            table_.BeginTransaction();
        }
        ~ScopeTransaction()
        {
            table_.EndTransaction();
        }
    
    private:
        Table_ABC& table_;
    };
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
void DatabaseUpdater::Update( const ASN1T_MsgUnitAttributes& msg )
{    
    Table_ABC& table = database_.OpenTable( "UnitForces" );    
    std::stringstream query;
    query << "Public_OID=" << msg.oid;
    ScopeTransaction transaction( table );
    if( Row_ABC* row = table.Find( query.str() ) )
    {
        ScopeEditor editor( database_ );
        if( msg.m.vitessePresent )
            row->SetField( "Speed", FieldVariant( msg.vitesse ) );
        if( msg.m.etat_operationnelPresent )
            row->SetField( "OpsState", FieldVariant( msg.etat_operationnel_brut ) );
        if( msg.m.positionPresent )
            row->SetShape( Point( msg.position ) );
        table.UpdateRow( *row );
    }    
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgUnitKnowledgeUpdate& msg )
{    
    Table_ABC& table = database_.OpenTable( "KnowledgeUnits" );
    std::stringstream query;
    query << "Public_OID=" << msg.oid;
    ScopeTransaction transaction( table );
    if( Row_ABC* row = table.Find( query.str() ) )
    {
        ScopeEditor editor( database_ );        
        if( msg.m.speedPresent )
            row->SetField( "Speed", FieldVariant( msg.speed ) );
        if( msg.m.identification_levelPresent )
            row->SetField( "IdentificationLevel", FieldVariant( msg.identification_level ) );
        if( msg.m.max_identification_levelPresent )
            UpdateSymbol( *row, model_.GetAgentKnowledges(), msg.oid );
        if( msg.m.positionPresent )
            row->SetShape( Point( msg.position ) );
        table.UpdateRow( *row );
    }    
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyUnit
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::DestroyUnit( const ASN1T_MsgUnitDestruction& msg )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( "UnitForces" );
    std::stringstream query;
    query << "Public_OID=" << msg;
    table.DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::Update
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void DatabaseUpdater::Update( const ASN1T_MsgUnitKnowledgeDestruction& msg )
{
    ScopeEditor editor( database_ );
    Table_ABC& table = database_.OpenTable( "KnowledgeUnits" );
    std::stringstream query;
    query << "Public_OID=" << msg.oid;
    table.DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: DatabaseUpdater::DestroyObject
// Created: SBO 2007-09-27
// -----------------------------------------------------------------------------
void DatabaseUpdater::DestroyObject( const ASN1T_MsgObjectDestruction& msg )
{
    ScopeEditor editor( database_ );
    std::stringstream ss;
    ss << "Public_OID=" << msg;
    database_.OpenTable( "TacticalObjectPoint" ).DeleteRows( ss.str() );
    database_.OpenTable( "TacticalObjectLine" ) .DeleteRows( ss.str() );
    database_.OpenTable( "TacticalObjectArea" ) .DeleteRows( ss.str() );
}
