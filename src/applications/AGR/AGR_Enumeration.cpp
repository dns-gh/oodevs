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
// Created: AGN 2004-04-26
// -----------------------------------------------------------------------------
void AGR_Enumeration::Read( xml::xistream& xis )
{
    xis >> xml::list( "xsd:enumeration", *this, &AGR_Enumeration::ReadValue );
}

// -----------------------------------------------------------------------------
// Name: AGR_Enumeration::ReadValue
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
void AGR_Enumeration::ReadValue( xml::xistream& xis )
{
    valueList_.push_back( xml::attribute< std::string >( xis, "value" ) );
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
