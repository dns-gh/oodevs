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

#include "dispatcher/AgentTypes.h"
#include "dispatcher/AgentType.h"

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
    // $$$$ JCR 2007-09-05: Rustine pour NMSG-049
    const kernel::AgentType& type = model_.GetAgentTypes().Get( asn.type_pion );
    const std::string& name = type.GetName();
        
    if ( name.substr( 0, 2 ) == "HS" )
        return styles_.GetStyle( name );

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
    switch ( asn.type ) 
    {
    case EnumObjectType::bouchon_mines:
        return styles_.GetStyle( "HS - Explosion" );
    case EnumObjectType::nuage_nbc:
        return styles_.GetStyle( "HS - Hazmat" );
    }
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
    // $$$$ JCR 2007-09-05: Rustine pour NMSG-049
    const kernel::AgentType& type = model_.GetAgentTypes().Get( asn.oid );
    const std::string& name = type.GetName();
    if ( name.substr( 0, 2 ) == "HS" )
        return styles_.GetStyle( name );

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
    // $$$$ JCR 2007-09-05: Rustine pour NMSG-049
    const kernel::AgentType& type = model_.GetAgentTypes().Get( asn.type_automate );
    const std::string& name = type.GetName();
    if ( name.substr( 0, 2 ) == "HS" )
        return styles_.GetStyle( name );

//    if( const dispatcher::AgentKnowledge* knowledge = model_.GetAgentKnowledges().Find( asn.oid ) )
//        return FormatSymbol( knowledge->BuildSymbol() ).c_str();
    return "";
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgLimaCreation& /*asn*/ ) const
{
    return styles_.GetStyle( "Lima" );
}

// -----------------------------------------------------------------------------
// Name: SymbolEditor::GetSymbol
// Created: JCR 2007-09-04
// -----------------------------------------------------------------------------
std::string SymbolEditor::GetSymbol( const ASN1T_MsgLimitCreation& /*asn*/ ) const
{
    return styles_.GetStyle( "Limit" );
}
