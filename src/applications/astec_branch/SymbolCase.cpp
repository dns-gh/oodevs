// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "SymbolCase.h"
#include "SymbolRule.h"
#include "SymbolRequest.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: SymbolCase constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolCase::SymbolCase( xml::xistream& xis )
{
    xis >> attribute( "value", value_ )
        >> list( "rule", *this, &SymbolCase::ReadRule );
}
    
// -----------------------------------------------------------------------------
// Name: SymbolCase destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolCase::~SymbolCase()
{
    for( CIT_Rules it = rules_.begin(); it != rules_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::ReadRule
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolCase::ReadRule( xml::xistream& xis )
{
    rules_.push_back( new SymbolRule( xis ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolCase::Evaluate
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
std::string SymbolCase::Evaluate( const std::string& ruleName, const SymbolRequest& request ) const
{
    if( !request.Matches( ruleName, value_ ) )
        return "";

    for( CIT_Rules it = rules_.begin(); it != rules_.end(); ++it )
    {
        const std::string value = (*it)->Evaluate( request );
        if( !value.empty() )
            return value;
    }
    return "";        
}
