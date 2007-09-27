// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ScopeEditor.h"

#include "Table_ABC.h"
#include "Row_ABC.h"
#include "SymbolEditor.h"
#include "ReportFactory.h"
#include "Line.h"
#include "Point.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Model.h"
#include "tools/App6Symbol.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: ScopeEditor constructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::ScopeEditor( const dispatcher::Model& model, const ReportFactory& reportFactory )
    : model_        ( model )
    , reportFactory_( reportFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor destructor
// Created: JCR 2007-04-30
// -----------------------------------------------------------------------------
ScopeEditor::~ScopeEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgUnitCreation& msg )
{
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( msg.oid ) );
    row.SetField( "Parent_OID", FieldVariant( msg.oid_automate ) );
    row.SetField( "Name"      , FieldVariant( std::string( msg.nom ) ) );
    SymbolEditor( model_ ).Update( row, msg );
    row.SetShape( Point() );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgUnitKnowledgeCreation& msg )
{
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID"  , FieldVariant( msg.oid ) );
    row.SetField( "Group_OID"   , FieldVariant( msg.oid_groupe_possesseur ) );
    row.SetField( "RealUnit_OID", FieldVariant( msg.oid_unite_reelle ) );

    if( const dispatcher::Agent* realAgent = model_.GetAgents().Find( msg.oid_unite_reelle ) )
        row.SetField( "ObserverAffiliation", FieldVariant( tools::app6::GetAffiliation( realAgent->BuildSymbol() ) ) );
    SymbolEditor( model_ ).Update( row, msg );
    row.SetShape( Point() );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgLimitCreation& msg )
{
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( msg.oid ) );
    row.SetField( "Name"      , FieldVariant( std::string( msg.tactical_line.name ) ) );
    SymbolEditor( model_ ).Update( row, msg );
    row.SetShape( Line( msg.tactical_line.geometry.coordinates ) );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgLimaCreation& msg )
{
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( msg.oid ) );
    row.SetField( "Name"      , FieldVariant( std::string( msg.tactical_line.name ) ) );
    SymbolEditor( model_ ).Update( row, msg );
    row.SetShape( Line( msg.tactical_line.geometry.coordinates ) );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgObjectCreation& msg )
{    
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( msg.oid ) );
    row.SetField( "Name"      , FieldVariant( std::string( msg.name ) ) );
    SymbolEditor( model_ ).Update( row, msg );
    switch ( msg.location.type )
    {
    case EnumLocationType::point: row.SetShape( Point( msg.location.coordinates.elem[ 0 ] ) ); break;
    case EnumLocationType::line:  row.SetShape( Line( msg.location.coordinates ) ); break;
    default:                      /*row.SetShape( Location( asn.location ) );*/ break; // $$$$ SBO 2007-08-31: TODO
    }
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgReport& msg )
{    
    Row_ABC& row = table.CreateRow();
    row.SetField( "unit_id", FieldVariant( msg.oid ) );
    row.SetField( "message", FieldVariant( reportFactory_.CreateMessage( msg ) ) );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgFormationCreation& asn )
{    
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( asn.oid ) );
    row.SetField( "Parent_OID", FieldVariant( asn.oid_formation_parente ) );
    row.SetField( "Name"      , FieldVariant( std::string( asn.nom ) ) );
    SymbolEditor( model_ ).Update( row, asn );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgAutomatCreation& asn )
{    
    Row_ABC& row = table.CreateRow();
    row.SetField( "Public_OID", FieldVariant( asn.oid ) );
    if( asn.oid_parent.t == T_MsgAutomatCreation_oid_parent_formation )
        row.SetField( "Parent_OID", FieldVariant( asn.oid_parent.u.formation ) );
    else
        row.SetField( "Parent_OID", FieldVariant( asn.oid_parent.u.automate ) );
    row.SetField( "Name"      , FieldVariant( std::string( asn.nom ) ) );
    SymbolEditor( model_ ).Update( row, asn );
    table.UpdateRow( row );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgUnitAttributes& msg )
{
    std::stringstream query;
    query << "Public_OID=" << msg.oid;
    if( Row_ABC* row = table.Find( query.str() ) )
    {
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
// Name: ScopeEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgUnitKnowledgeUpdate& msg )
{
    std::stringstream query;
    query << "Public_OID=" << msg.oid;
    if( Row_ABC* row = table.Find( query.str() ) )
    {
        if( msg.m.speedPresent )
            row->SetField( "Speed", FieldVariant( msg.speed ) );
        if( msg.m.identification_levelPresent )
            row->SetField( "IdentificationLevel", FieldVariant( msg.identification_level ) );
        if( msg.m.max_identification_levelPresent )
            SymbolEditor( model_ ).Update( *row, msg );
        if( msg.m.positionPresent )
            row->SetShape( Point( msg.position ) );
        table.UpdateRow( *row );
    }
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgUnitDestruction& msg )
{
    std::stringstream query;
    query << "Public_OID=" << msg;
    table.DeleteRows( query.str() );
}

// -----------------------------------------------------------------------------
// Name: ScopeEditor::Update
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void ScopeEditor::Update( Table_ABC& table, const ASN1T_MsgUnitKnowledgeDestruction& msg )
{
    std::stringstream query;
    query << "Public_OID=" << msg.oid;
    table.DeleteRows( query.str() );
}
