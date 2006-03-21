// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "SymbolRequest.h"
#include "AgentType.h"
#include "AgentNature.h"

// -----------------------------------------------------------------------------
// Name: SymbolRequest constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRequest::SymbolRequest( const AgentType& type )
    : type_( type )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: SymbolRequest destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
SymbolRequest::~SymbolRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolRequest::Matches
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
bool SymbolRequest::Matches( const std::string& name, const std::string& value ) const
{
    std::string values = type_.GetNature().Retrieve( name ); // $$$$ SBO 2006-03-20: can be a comma separated list of values

    unsigned int pos = 0;
    while( ( pos = values.find_first_of( ", " ) ) != values.npos )
    {
        const std::string val = values.substr( pos, values.find_first_of( ", " ) );
        while( values[ ++pos ] == ',' || values[ pos ] == ' ' )
            ;
        values = values.substr( pos );
        if( val == value )
            return true;
    }
    return values == value;
}
