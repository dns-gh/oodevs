// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "LocalizedString.h"
#include "Language.h"

using namespace kernel;

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

// -----------------------------------------------------------------------------
// Name: LocalizedString constructor
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
LocalizedString::LocalizedString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalizedString constructor
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
LocalizedString::LocalizedString( const std::string& key )
    : key_( key )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalizedString destructor
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
LocalizedString::~LocalizedString()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Key
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
const std::string& LocalizedString::Key() const
{
    return key_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Value
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
const std::string& LocalizedString::Value() const
{
    return Language::IsCurrentDefault() || key_.empty() ? key_ : values_.at( Language::Current() ).value_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Value
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
const std::string& LocalizedString::Value( const std::string& language ) const
{
    return Language::IsDefault( language ) || key_.empty() ? key_ : values_.at( language ).value_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Type
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
E_TranslationType LocalizedString::Type() const
{
    return Language::IsCurrentDefault() || key_.empty() ? eTranslationType_None : values_.at( Language::Current() ).type_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Type
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
E_TranslationType LocalizedString::Type( const std::string& language ) const
{
    return Language::IsDefault( language ) || key_.empty() ? eTranslationType_None : values_.at( language ).type_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::SetKey
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
void LocalizedString::SetKey( const std::string& key )
{
    key_ = key;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::SetValue
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
void LocalizedString::SetValue( const std::string& value )
{
    if( Language::IsCurrentDefault() )
        key_ = value;
    else
        values_[ Language::Current() ].value_ = value;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::SetValue
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
void LocalizedString::SetValue( const std::string& language, const std::string& value )
{
    if( Language::IsDefault( language ) )
        key_ = value;
    else
        values_[ language ].value_ = value;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::SetType
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
void LocalizedString::SetType( E_TranslationType type )
{
    if( !Language::IsCurrentDefault() )
        values_[ Language::Current() ].type_ = type;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::SetType
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
void LocalizedString::SetType( const std::string& language, E_TranslationType type )
{
    if( !Language::IsDefault( language ) )
        values_[ language ].type_ = type;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::InitEmptyValues
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
void LocalizedString::InitEmptyValues( const std::vector< Language >& languages )
{
    for( auto it = languages.begin(); it != languages.end(); ++it )
    {
        auto valueIt = values_.find( it->GetShortName() );
        if( valueIt == values_.end() /*|| valueIt->second.value_.empty()*/ )
        {
            values_[ it->GetShortName() ].value_ = "";
            values_[ it->GetShortName() ].type_ = eTranslationType_Unfinished;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::CopyValues
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
void LocalizedString::CopyValues( const LocalizedString& other )
{
    for( auto it = other.values_.begin(); it != other.values_.end(); ++it )
        values_[ it->first ] = it->second;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::operator ==
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
bool LocalizedString::operator==( const LocalizedString& other ) const
{
    bool result = Key() == other.Key();
    for( auto it = values_.begin(); result && it != values_.end(); ++it )
        result = result && it->second.value_ == other.Value( it->first ) && it->second.type_ == other.Type( it->first );
    return result;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::operator !=
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
bool LocalizedString::operator!=( const LocalizedString& other ) const
{
    return !( *this == other );
}
