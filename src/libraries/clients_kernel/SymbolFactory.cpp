// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "SymbolFactory.h"
#include "SymbolRequest.h"
#include "SymbolRule.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: SymbolFactory constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolFactory::SymbolFactory( xml::xistream& xis )
{
    xis >> start( "rulesets" )
            >> start( "automats" )
				>> list( "rule", *this, &SymbolFactory::ReadAutomatRule )
            >> end()
            >> start( "symbols" )
                >> list( "rule", *this, &SymbolFactory::ReadRule, symbolRules_ )
            >> end()
            >> start( "levels" )
                >> list( "rule", *this, &SymbolFactory::ReadRule, levelRules_ )
            >> end();
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolFactory::~SymbolFactory()
{
    for( CIT_Rules it = symbolRules_.begin(); it != symbolRules_.end(); ++it )
        delete *it;
    for( CIT_Rules it = levelRules_.begin(); it != levelRules_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::ReadRule
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolFactory::ReadRule( xml::xistream& xis, T_Rules& rules ) const
{
    rules.push_back( new SymbolRule( xis ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::ReadAutomatRule
// Created: SBO 2006-03-21
// -----------------------------------------------------------------------------
void SymbolFactory::ReadAutomatRule( xml::xistream& xis )
{
    xis >> attribute( "default", automatSymbol_ );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateSymbol
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateSymbol( const AgentNature& nature ) const
{
    return CreateSymbolFromRules( nature, symbolRules_ );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateLevelSymbol
// Created: SBO 2006-03-21
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateLevelSymbol( const AgentNature& nature ) const
{
    return CreateSymbolFromRules( nature, levelRules_ );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateSymbolFromRules
// Created: SBO 2006-03-21
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateSymbolFromRules( const AgentNature& nature, const T_Rules& rules ) const
{
    SymbolRequest request( nature );

    for( CIT_Rules it = rules.begin(); it != rules.end(); ++it )
    {
        std::string value = (*it)->Evaluate( request );
        if( !value.empty() )
            return value;
    }
    return "";
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateAutomatSymbol
// Created: SBO 2006-03-21
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateAutomatSymbol() const
{
    return automatSymbol_;
}
