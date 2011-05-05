// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AttributeType.h"
#include "EntryLabelType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AttributeType constructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
AttributeType::AttributeType( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name" ) )
{
    SetType( xis.attribute< std::string >( "type" ) );
    ReaderHelper( extends_, "extends", xis );
    ReaderHelper( dictionary_, "dictionary", xis );
    ReaderHelper( minLength_, "min-length", xis );
    ReaderHelper( maxLength_, "max-length", xis );
    ReaderHelper( dictionaryKind_, "dictionary-kind", xis );
    ReaderHelper( dictionaryLanguage_, "dictionary-language", xis );
    xis >> xml::optional >> xml::start( "labels" )
            >> xml::list( "label", *this, &AttributeType::ReadLabel )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AttributeType destructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
AttributeType::~AttributeType()
{
    for( CIT_Entries it = labels_.begin(); it != labels_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: AttributeType::Extends
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
bool AttributeType::Extends( const std::string& type ) const
{
    if( !extends_.IsSet() )
        throw std::exception( "Needed \"extends\"not defined in Extensions.xml" );
    const std::string& extends = extends_;
    return extends.find( type ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: AttributeType::GetLabel
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
const std::string& AttributeType::GetLabel( const std::string& language, const std::string& kind ) const
{
    for( CIT_Entries it = labels_.begin(); it != labels_.end(); ++it )
        if( ( *it )->Matches( language, kind ) )
            return ( *it )->GetText();
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AttributeType::GetMinMaxLength
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
void AttributeType::GetMinMaxLength( int& min, int& max ) const
{
    min = minLength_.IsSet() ? static_cast< int >( minLength_ ) : -1;
    max = maxLength_.IsSet() ? static_cast< int >( maxLength_ ) : -1;
}

// -----------------------------------------------------------------------------
// Name: AttributeType::GetDictionaryValues
// Created: JSR 2010-10-05
// -----------------------------------------------------------------------------
void AttributeType::GetDictionaryValues( std::string& dictionary, std::string& kind, std::string& language ) const
{
    if( dictionary_.IsSet() )
        dictionary = dictionary_;
    if( dictionaryKind_.IsSet() )
        kind = dictionaryKind_;
    if( dictionaryLanguage_.IsSet() )
        language = dictionaryLanguage_;
}

// -----------------------------------------------------------------------------
// Name: AttributeType::SetType
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void AttributeType::SetType( const std::string& type )
{
    if( type == "boolean" )
        type_ = ETypeBoolean;
    else if( type == "string" )
        type_ = ETypeString;
    else if( type == "alphanumeric" )
        type_ = ETypeAlphanumeric;
    else if( type == "numeric" )
        type_ = ETypeNumeric;
    else if( type == "dictionary" )
        type_ = ETypeDictionary;
    else if( type == "loosy-dictionary" )
        type_ = ETypeLoosyDictionary;
    else if( type == "diffusion-list" )
        type_ = ETypeDiffusionList;
    else
        throw std::exception( "Bad attribute type" );
}

// -----------------------------------------------------------------------------
// Name: AttributeType::ReadLabel
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void AttributeType::ReadLabel( xml::xistream& xis )
{
    labels_.push_back( new EntryLabelType( xis ) );
}
