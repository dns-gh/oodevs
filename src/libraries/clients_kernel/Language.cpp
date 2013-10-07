// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Language.h"

using namespace kernel;
std::string Language::current_;

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
Language::Language( xml::xistream& xis )
    : name_( xis.attribute( "name", "" ) )
    , code_( xis.attribute( "code", "" ) )
{
    if( name_.empty() || code_.empty() )
        throw MASA_EXCEPTION( QString( "Incomplete language '%1' / '%2'" ).arg( name_.c_str() ).arg( code_.c_str() ).toStdString() );
    else if( code_.size() != 2 )
        throw MASA_EXCEPTION( QString( "Invalid language code '%1'" ).arg( code_.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: Language destructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
Language::~Language()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Language::GetName
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
const std::string& Language::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Language::GetCode
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
const std::string& Language::GetCode() const
{
    return code_;
}

// -----------------------------------------------------------------------------
// Name: Language::Current
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
const std::string& Language::Current()
{
    return current_;
}

// -----------------------------------------------------------------------------
// Name: Language::SetCurrent
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
void Language::SetCurrent( const std::string& language )
{
    current_ = language;
}
