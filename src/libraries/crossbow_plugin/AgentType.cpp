// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "AgentType.h"
#include "xeumeuleu/xml.h"
#include "AgentNature.h"
#include "SymbolFactory.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentType constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::AgentType( xml::xistream& xis, const SymbolFactory& symbolFactory )
    : nature_ ( 0 )
{        
    xis >> attribute( "nom", name_ )
        >> attribute( "type", type_ )
        >> content( "MosID", id_ )
        >> content( "ModeleDecisionnel", model_ );    
    
    std::auto_ptr< AgentNature > nature( new AgentNature( xis ) );
    symbol_      = symbolFactory.CreateSymbol( nature->GetNature() );
    levelSymbol_ = symbolFactory.CreateLevelSymbol( nature->GetLevel() );
    hqSymbol_    = symbolFactory.CreateAutomatSymbol();
    nature_ = nature.release();
}

// -----------------------------------------------------------------------------
// Name: AgentType destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
AgentType::~AgentType()
{
    delete nature_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long AgentType::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const std::string& AgentType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetNature
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
const AgentNature& AgentType::GetNature() const
{
    return *nature_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
const std::string& AgentType::GetSymbol() const
{
    return symbol_;
}
    
// -----------------------------------------------------------------------------
// Name: AgentType::GetLevelSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
const std::string& AgentType::GetLevelSymbol() const
{
    return levelSymbol_;
}

// -----------------------------------------------------------------------------
// Name: AgentType::GetHQSymbol
// Created: AGE 2006-10-24
// -----------------------------------------------------------------------------
const std::string& AgentType::GetHQSymbol() const
{
    return hqSymbol_;
}