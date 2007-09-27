// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "SymbolEditor.h"
#include "Row_ABC.h"
#include "dispatcher/Model.h"
#include "dispatcher/Side.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/Object.h"
#include "dispatcher/Limit.h"
#include "dispatcher/Lima.h"

using namespace crossbow;

namespace
{
    std::string FormatSymbol( const std::string& symbol )
    {
        std::string result( symbol.size(), '*' );
        std::transform( symbol.begin(), symbol.end(), result.begin(), toupper );
        std::replace( result.begin(), result.end(), '*', '-' );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::SymbolEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
SymbolEditor::SymbolEditor( const dispatcher::Model& model )
    : model_ ( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::~SymbolEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
SymbolEditor::~SymbolEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void SymbolEditor::Update( Row_ABC& row, const ASN1T_MsgLimitCreation& msg )
{
    if( const dispatcher::Limit* element = model_.GetLimits().Find( msg.oid ) )
        row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( element->BuildSymbol() ) ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void SymbolEditor::Update( Row_ABC& row, const ASN1T_MsgLimaCreation& msg )
{
    if( const dispatcher::Lima* element = model_.GetLimas().Find( msg.oid ) )
        row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( element->BuildSymbol() ) ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void SymbolEditor::Update( Row_ABC& row, const ASN1T_MsgObjectCreation& msg )
{
    if( const dispatcher::Object* element = model_.GetObjects().Find( msg.oid ) )
        row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( element->BuildSymbol() ) ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void SymbolEditor::Update( Row_ABC& row, const ASN1T_MsgUnitCreation& msg )
{
    if( const dispatcher::Agent* element = model_.GetAgents().Find( msg.oid ) )
        row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( element->BuildSymbol() ) ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void SymbolEditor::Update( Row_ABC& row, const ASN1T_MsgUnitKnowledgeCreation& msg )
{
    std::string symbol( "SUZP********---" );
    row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( symbol ) ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void SymbolEditor::Update( Row_ABC& row, const ASN1T_MsgUnitKnowledgeUpdate& msg )
{
    if( const dispatcher::AgentKnowledge* element = model_.GetAgentKnowledges().Find( msg.oid ) )
        row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( element->BuildSymbol() ) ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void SymbolEditor::Update( Row_ABC& row, const ASN1T_MsgFormationCreation& msg )
{
    if( const dispatcher::Formation* element = model_.GetFormations().Find( msg.oid ) )
        row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( element->BuildSymbol() ) ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Update
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void SymbolEditor::Update( Row_ABC& row, const ASN1T_MsgAutomatCreation& msg )
{
    if( const dispatcher::Automat* element = model_.GetAutomats().Find( msg.oid ) )
        row.SetField( "Symbol_ID", FieldVariant( FormatSymbol( element->BuildSymbol() ) ) );
}
