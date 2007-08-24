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
#include "ScopeEditor.h"
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
SymbolEditor::SymbolEditor( ScopeEditor& scope, const dispatcher::Model& model )
    : scope_ ( scope )
    , model_ ( model )
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
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgLimitCreation& msg )
{
    if( const dispatcher::Limit* limit = model_.GetLimits().Find( msg.oid ) )
        scope_.Write(spFeature, CComBSTR( L"Symbol_ID" ), FormatSymbol( limit->BuildSymbol() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgLimaCreation& msg )
{
    if( const dispatcher::Lima* lima = model_.GetLimas().Find( msg.oid ) )
        scope_.Write(spFeature, CComBSTR( L"Symbol_ID" ), FormatSymbol( lima->BuildSymbol() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spRow, const ASN1T_MsgObjectCreation& msg )
{
    if( const dispatcher::Object* object = model_.GetObjects().Find( msg.oid ) )
        scope_.Write(spRow, CComBSTR( L"Symbol_ID" ), FormatSymbol( object->BuildSymbol() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-06-12
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IRowBufferPtr spRow, const ASN1T_MsgFormationCreation& asn )
{
    if( const dispatcher::Formation* formation = model_.GetFormations().Find( asn.oid ) )
        scope_.Write(spRow, CComBSTR( L"Symbol_ID" ), FormatSymbol( formation->BuildSymbol() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgUnitCreation& asn )
{
    if( const dispatcher::Agent* agent = model_.GetAgents().Find( asn.oid ) )
        scope_.Write(spFeature, CComBSTR( L"Symbol_ID" ), FormatSymbol( agent->BuildSymbol() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-06-12
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IRowBufferPtr spRow, const ASN1T_MsgAutomatCreation& asn )
{
    if( const dispatcher::Automat* automat = model_.GetAutomats().Find( asn.oid ) )
        scope_.Write(spRow, CComBSTR( L"Symbol_ID" ), FormatSymbol( automat->BuildSymbol() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgUnitKnowledgeCreation& /*asn*/ )
{
    // $$$$ SBO 2007-08-22: TODO !
    std::string symbol( "SUZP********---" );
    scope_.Write( spFeature, CComBSTR( L"Symbol_ID" ), FormatSymbol( symbol ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::Write
// Created: JCR 2007-05-23
// -----------------------------------------------------------------------------
void SymbolEditor::Write( IFeaturePtr spFeature, const ASN1T_MsgUnitKnowledgeUpdate& asn )
{
    if( const dispatcher::AgentKnowledge* knowledge = model_.GetAgentKnowledges().Find( asn.oid ) )
        scope_.Write( spFeature, CComBSTR( L"Symbol_ID" ), FormatSymbol( knowledge->BuildSymbol() ).c_str() );
}
