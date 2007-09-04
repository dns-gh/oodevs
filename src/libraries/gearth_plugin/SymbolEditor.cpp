// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "SymbolEditor.h"
#include "StyleEditor.h"

#include "dispatcher/Model.h"
#include "dispatcher/Side.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/Object.h"
#include "dispatcher/Limit.h"
#include "dispatcher/Lima.h"

using namespace gearth;

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
SymbolEditor::SymbolEditor( const StyleEditor& styleEditor, const dispatcher::Model& model )
    : model_ ( model )
    , styles_ ( styleEditor )
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
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgUnitCreation& asn ) const
{
    if( const dispatcher::Agent* agent = model_.GetAgents().Find( asn.oid ) )
        return FormatSymbol( agent->BuildSymbol() ).c_str();
    return "";
}
    
// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgUnitKnowledgeCreation& asn ) const
{    
    std::string symbol( "SUZP********---" );
    if( const dispatcher::AgentKnowledge* knowledge = model_.GetAgentKnowledges().Find( asn.oid ) )
        return FormatSymbol( knowledge->BuildSymbol() ).c_str();
    return "";
}
    
// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgObjectCreation& asn ) const
{
    if( const dispatcher::Object* object = model_.GetObjects().Find( asn.oid ) )
        return FormatSymbol( object->BuildSymbol() ).c_str();
    return "";
}
    
// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgFormationCreation& asn ) const
{
    if( const dispatcher::Formation* formation = model_.GetFormations().Find( asn.oid ) )
        FormatSymbol( formation->BuildSymbol() ).c_str();
    return "";
}
    
// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgAutomatCreation& asn ) const
{
    if( const dispatcher::AgentKnowledge* knowledge = model_.GetAgentKnowledges().Find( asn.oid ) )
        return FormatSymbol( knowledge->BuildSymbol() ).c_str();
    return "";
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgLimaCreation& /*asn*/ ) const
{
    return "Lima";
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgLimitCreation& /*asn*/ ) const
{
    return "Limit";
}
