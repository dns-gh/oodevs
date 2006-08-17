// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_kernel_pch.h"
#include "SymbolRule.h"
#include "SymbolCase.h"
#include "SymbolRequest.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: SymbolRule constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRule::SymbolRule( xml::xistream& xis )
{
    xis >> optional() >> attribute( "name", name_ )
        >> optional() >> attribute( "default", default_ );
    
    if( !name_.empty() && !default_.empty() )
        throw std::runtime_error( "Symbol rule cannot specify both 'name' and 'default'" );

    xis >> optional() >> list( "case", *this, & SymbolRule::ReadCase );
}
    
// -----------------------------------------------------------------------------
// Name: SymbolRule destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRule::~SymbolRule()
{
    for( CIT_Cases it = cases_.begin(); it != cases_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::ReadCase
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void SymbolRule::ReadCase( xml::xistream& xis )
{
    cases_.push_back( new SymbolCase( xis ) );
}

// -----------------------------------------------------------------------------
// Name: SymbolRule::Evaluate
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
std::string SymbolRule::Evaluate( const SymbolRequest& request ) const
{
    if( name_.empty() && cases_.empty() ) // empty rule: return default_
        return default_;

    for( CIT_Cases it = cases_.begin(); it != cases_.end(); ++it )
    {
        const std::string value = (*it)->Evaluate( name_, request );
        if( !value.empty() )
            return value;
    }
    return ""; // no match
}
