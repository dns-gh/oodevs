// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_Enumeration.h"

#include <boost/algorithm/string.hpp>
#include <cstdio>
#include <assert.h>

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration constructor
/** @param  strName 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
AGR_Enumeration::AGR_Enumeration( const std::string& strName )
    : strName_( strName )
{
    
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration destructor
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
AGR_Enumeration::~AGR_Enumeration()
{
    
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::Read
/** @param  input 
*/
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void AGR_Enumeration::Read( MT_InputArchive_ABC& input )
{
    while( input.NextListElement() )
    {
        input.Section( "xsd:enumeration" );
        std::string strValue;
        input.ReadAttribute( "value", strValue );
        assert( std::find( valueList_.begin(), valueList_.end(), strValue ) == valueList_.end() );
        valueList_.push_back( strValue );
        input.EndSection(); // xsd:enumeration
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::HumanName
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
std::string AGR_Enumeration::HumanName( const std::string& strValue ) const
{
    std::string strResult = strValue;

    boost::replace_all( strResult, "_", " " );

    if( boost::is_lower()( strResult[ 0 ] ) )
        strResult[ 0 ] = strResult[ 0 ] + 'A' - 'a';

    std::string::iterator prevIt = strResult.begin();
    for( std::string::iterator it = strResult.begin(); it != strResult.end(); )
    {
        if( boost::is_upper()( *it ) && boost::is_lower()( *prevIt ) )
            it = strResult.insert( it, ' ' );

        prevIt = it;
        ++it;
    }
    boost::trim( strResult );
    return strResult;
}
