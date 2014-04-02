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
#include "ExtensionDependency.h"
#include "Profile_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AttributeType constructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
AttributeType::AttributeType( xml::xistream& xis )
    : name_    ( xis.attribute< std::string >( "name" ) )
    , operator_( EOperatorAND )
{
    SetType( xis.attribute< std::string >( "type" ) );
    ReaderHelper( extends_, "extends", xis );
    ReaderHelper( dictionary_, "dictionary", xis );
    ReaderHelper( minLength_, "min-length", xis );
    ReaderHelper( maxLength_, "max-length", xis );
    ReaderHelper( dictionaryKind_, "dictionary-kind", xis );
    ReaderHelper( dictionaryLanguage_, "dictionary-language", xis );
    ReaderHelper( supervisor_, "supervisor", xis );
    std::string op;
    xis >> xml::optional >> xml::start( "dependencies" )
            >> xml::attribute( "logical", op )
            >> xml::list( "dependency", *this, &AttributeType::ReadDependency )
        >> xml::end
        >> xml::optional >> xml::start( "labels" )
            >> xml::list( "label", *this, &AttributeType::ReadLabel )
        >> xml::end;
    if( !op.empty() )
    {
        if( op == "or" )
            operator_ = EOperatorOR;
        else if( op != "and" )
            throw MASA_EXCEPTION( "unknown logical operator, use 'and' or 'or'." );
    }
}

// -----------------------------------------------------------------------------
// Name: AttributeType destructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
AttributeType::~AttributeType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributeType::Extends
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
bool AttributeType::Extends( const std::string& type ) const
{
    if( !extends_.IsSet() )
        throw MASA_EXCEPTION( "Needed \"extends\"not defined in Extensions.xml" );
    const std::string& extends = extends_;
    return extends.find( type ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: AttributeType::GetLabel
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
const std::string& AttributeType::GetLabel( const std::string& language, const std::string& kind ) const
{
    for( auto it = labels_.begin(); it != labels_.end(); ++it )
        if( it->Matches( language, kind ) )
            return it->GetText();
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
        throw MASA_EXCEPTION( "Bad attribute type" );
}

// -----------------------------------------------------------------------------
// Name: AttributeType::ReadLabel
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
void AttributeType::ReadLabel( xml::xistream& xis )
{
    labels_.push_back( new EntryLabelType( xis ) );
}

// -----------------------------------------------------------------------------
// Name: AttributeType::ReadDependency
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
void AttributeType::ReadDependency( xml::xistream& xis )
{
    dependencies_.push_back( new ExtensionDependency( xis ) );
}

// -----------------------------------------------------------------------------
// Name: AttributeType::IsActive
// Created: ABR 2011-05-03
// -----------------------------------------------------------------------------
bool AttributeType::IsActive( const std::map< std::string, std::string >& extensions ) const
{
    if( dependencies_.empty() )
        return true;
    bool active = operator_ == EOperatorAND;
    bool hasChanged = false;
    for( auto dep = dependencies_.begin(); dep != dependencies_.end(); ++dep )
        for( auto ext = extensions.begin(); ext != extensions.end(); ++ext )
            if( dep->GetName() == ext->first )
            {
                if( operator_ == EOperatorAND )
                    active = active && dep->Allow( ext->second );
                else if( operator_ == EOperatorOR )
                    active = active || dep->Allow( ext->second );
                hasChanged = true;
                break;
            }
    return hasChanged ? active : false;
}

// -----------------------------------------------------------------------------
// Name: AttributeType::IsEditable
// Created: SLI 2014-04-02
// -----------------------------------------------------------------------------
bool AttributeType::IsEditable( const kernel::Profile_ABC& profile ) const
{
    if( !supervisor_.IsSet() )
        return true;
    return supervisor_.Data() && profile.IsSupervision();
}
