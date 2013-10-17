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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: E_TranslationType::operator<<
// Created: ABR 2013-07-18
// -----------------------------------------------------------------------------
xml::xostream& kernel::operator<<( xml::xostream& xos, E_TranslationType type )
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
    std::string typeStr;
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
// Name: LocalizedString::CheckLanguageValidity
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
void LocalizedString::CheckLanguageValidity( const std::string& language ) const
{
    if( language.size() != 2 )
        throw MASA_EXCEPTION( "Invalid language: " + language );
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Value
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
const std::string& LocalizedString::Value( const std::string& language ) const
{
    CheckLanguageValidity( language );
    auto it = values_.find( language );
    if( it == values_.end() )
        throw MASA_EXCEPTION( "Language not initialized: " + language );
    return it->second.value_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Type
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
E_TranslationType LocalizedString::Type( const std::string& language ) const
{
    CheckLanguageValidity( language );
    auto it = values_.find( language );
    if( it == values_.end() )
        throw MASA_EXCEPTION( "Language not initialized: " + language );
    return it->second.type_;
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
void LocalizedString::SetValue( const std::string& language, const std::string& value )
{
    CheckLanguageValidity( language );
    values_[ language ].value_ = value;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::SetType
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
void LocalizedString::SetType( const std::string& language, E_TranslationType type )
{
    CheckLanguageValidity( language );
    values_[ language ].type_ = type;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::InitEmptyValues
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
bool LocalizedString::Initialize( const tools::LanguagesVector& languages )
{
    for( auto it = languages.begin(); it != languages.end(); ++it )
    {
        auto valueIt = values_.find( it->GetCode() );
        if( valueIt == values_.end() )
        {
            values_[ it->GetCode() ].value_ = "";
            values_[ it->GetCode() ].type_ = eTranslationType_Unfinished;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::CopyValues
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
void LocalizedString::CopyValues( const LocalizedString& other )
{
    values_ = other.values_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::operator ==
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
bool LocalizedString::operator==( const LocalizedString& other ) const
{
    return key_ == other.key_ && values_ == other.values_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::operator !=
// Created: ABR 2013-08-26
// -----------------------------------------------------------------------------
bool LocalizedString::operator!=( const LocalizedString& other ) const
{
    return !( *this == other );
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Translate
// Created: JSR 2013-10-04
// -----------------------------------------------------------------------------
const std::string& LocalizedString::Translate() const
{
    return Translate( tools::Language::Current() );
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::Translate
// Created: JSR 2013-10-04
// -----------------------------------------------------------------------------
const std::string& LocalizedString::Translate( const std::string& language ) const
{
    auto it = values_.find( language );
    if( it != values_.end() && it->second.type_ != eTranslationType_Unfinished && !it->second.value_.empty() )
        return it->second.value_;
    return key_;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::SwapKey
// Created: ABR 2013-10-04
// -----------------------------------------------------------------------------
bool LocalizedString::SwapKey( const std::string& oldKey, const std::string& newKey )
{
    values_[ oldKey ].value_ = key_;
    values_[ oldKey ].type_ = eTranslationType_None;
    auto it = values_.find( newKey );
    if( it == values_.end() )
        throw MASA_EXCEPTION( "New key doesn't have a translation value: " + newKey );
    if( !IsUnfinished( newKey ) )
        key_ = it->second.value_;
    values_.erase( it );
    return false;
}

// -----------------------------------------------------------------------------
// Name: LocalizedString::IsUnfinished
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
bool LocalizedString::IsUnfinished( const std::string& language ) const
{
    if( key_.empty() )
        return false;
    return values_.at( language ).type_ == eTranslationType_Unfinished;
}
