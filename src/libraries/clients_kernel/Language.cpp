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
#include <boost/algorithm/string.hpp>

using namespace kernel;

const std::string Language::default_ = "default";
std::string Language::current_ = "default";

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
Language::Language()
    : name_()
    , code_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
Language::Language( const Language& other )
    : name_( other.name_ )
    , code_( other.code_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
Language::Language( xml::xistream& xis )
    : name_()
    , code_()
{
    Read( xis );
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
// Name: Language::Read
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void Language::Read( xml::xistream& xis )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "code", code_ );
    code_ = boost::algorithm::to_lower_copy( code_ );
    if( name_.empty() || code_.empty() )
        throw MASA_EXCEPTION( QString( "Incomplete language '%1' / '%2'" ).arg( name_.c_str() ).arg( code_.c_str() ).toStdString() );
    else if( code_.size() != 2 )
        throw MASA_EXCEPTION( QString( "Invalid language shortname '%1'" ).arg( code_.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: Language::Write
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void Language::Write( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", name_ )
        << xml::attribute( "code", code_ );
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
// Name: Language::operator>>
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
xml::xistream& kernel::operator>>( xml::xistream& xis, Language& language )
{
    xis >> xml::start( "language" );
    language.Read( xis );
    xis >> xml::end;
    return xis;
}

// -----------------------------------------------------------------------------
// Name: Language::operator<<
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
xml::xostream& kernel::operator<<( xml::xostream& xos, const Language& language )
{
    xos << xml::start( "language" );
    language.Write( xos );
    xos << xml::end;
    return xos;
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
// Name: Language::Default
// Created: ABR 2013-08-29
// -----------------------------------------------------------------------------
const std::string& Language::Default()
{
    return default_;
}

// -----------------------------------------------------------------------------
// Name: Language::SetCurrent
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
void Language::SetCurrent( const std::string& language )
{
    current_ = language;
}

// -----------------------------------------------------------------------------
// Name: Language::IsDefault
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
bool Language::IsDefault( const std::string& language )
{
    return language == default_;
}

// -----------------------------------------------------------------------------
// Name: Language::IsCurrentDefault
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
bool Language::IsCurrentDefault()
{
    return IsDefault( current_ );
}
