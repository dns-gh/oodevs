// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "EntryLabelType.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntryLabelType constructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
EntryLabelType::EntryLabelType( xml::xistream& xis )
    : text_( xis.attribute< std::string >( "text" ) )
{
    if( xis.has_attribute( "language" ) )
        language_ = xis.attribute< std::string >( "language" );
    if( xis.has_attribute( "kind" ) )
        kind_ = xis.attribute< std::string >( "kind" );
}

// -----------------------------------------------------------------------------
// Name: EntryLabelType destructor
// Created: JSR 2010-10-01
// -----------------------------------------------------------------------------
EntryLabelType::~EntryLabelType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntryLabelType::Matches
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
bool EntryLabelType::Matches( const std::string& language, const std::string& kind ) const
{
    bool matchLanguage = language.empty() || !language_.IsSet() || static_cast< const std::string& >( language_ ).find( language ) != static_cast< const std::string& >( language_ ).npos;
    bool matchKind = kind.empty() || !kind_.IsSet() || static_cast< const std::string& >( kind_ ) ==  kind;
    return matchKind && matchLanguage;
}
    
// -----------------------------------------------------------------------------
// Name: EntryLabelType::GetText
// Created: JSR 2010-10-04
// -----------------------------------------------------------------------------
const std::string& EntryLabelType::GetText() const
{
    return text_;
}
