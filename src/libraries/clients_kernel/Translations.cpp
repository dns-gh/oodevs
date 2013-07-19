// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Translations.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Translations constructor
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
Translations::Translations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Translations destructor
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
Translations::~Translations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: std::vector< Translation >::iterator& Translations::find
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
std::vector< Translation >::iterator Translations::find( const std::string& key )
{
    for( auto it = begin(); it != end(); ++it )
        if( it->key_ == key )
            return it;
    return end();
}

// -----------------------------------------------------------------------------
// Name: std::vector< Translation >::const_iterator& Translations::find
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const std::vector< Translation >::const_iterator Translations::find( const std::string& key ) const
{
    for( auto it = begin(); it != end(); ++it )
        if( it->key_ == key )
            return it;
    return end();
}

// -----------------------------------------------------------------------------
// Name: Translations::operator[]
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
T_LocalizedTexts& Translations::operator[]( const std::string& key )
{
    for( auto it = begin(); it != end(); ++it )
        if( it->key_ == key )
            return it->values_;
    push_back( Translation( key ) );
    return back().values_;
}

// -----------------------------------------------------------------------------
// Name: Translations::operator[]
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const T_LocalizedTexts& Translations::operator[]( const std::string& key ) const
{
    for( auto it = begin(); it != end(); ++it )
        if( it->key_ == key )
            return it->values_;
    throw MASA_EXCEPTION( std::string( "Key not found: " ) + key );
}

// -----------------------------------------------------------------------------
// Name: Translations::at
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
const T_LocalizedTexts& Translations::at( const std::string& key ) const
{
    return ( *this )[ key ];
}

// -----------------------------------------------------------------------------
// Name: E_TranslationType::operator<<
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
xml::xostream& kernel::operator<<( xml::xostream& xos, const E_TranslationType& type )
{
    if( type == eTranslationType_Unfinished )
        xos << xml::attribute( "type", "unfinished" );
    return xos;
}

// -----------------------------------------------------------------------------
// Name: E_TranslationType::operator>>
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
xml::xistream& kernel::operator>>( xml::xistream& xis, E_TranslationType& type )
{
    std::string typeStr = "";
    xis >> xml::optional >> xml::attribute( "type", typeStr );
    type = ( typeStr.empty() ) ? eTranslationType_None : eTranslationType_Unfinished;
    return xis;
}
