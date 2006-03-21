// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
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
    xis >> start( "rules" )
            >> list( "rule", *this, ReadRule );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolFactory::~SymbolFactory()
{
    for( CIT_Rules it = rules_.begin(); it != rules_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::ReadRule
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolFactory::ReadRule( xml::xistream& xis )
{
    rules_.push_back( new SymbolRule( xis ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolFactory::CreateSymbol
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
std::string SymbolFactory::CreateSymbol( const AgentType& type ) const
{
    SymbolRequest request( type );
        
    for( CIT_Rules it = rules_.begin(); it != rules_.end(); ++it )
    {
        std::string value = (*it)->Evaluate( request );
        if( !value.empty() )
            return value;
    }
    return "";
}
