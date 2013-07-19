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

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
Language::Language()
    : name_()
    , shortname_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
Language::Language( const Language& other )
    : name_( other.name_ )
    , shortname_( other.shortname_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Language constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
Language::Language( xml::xistream& xis )
    : name_()
    , shortname_()
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
        >> xml::attribute( "shortname", shortname_ );
    shortname_ = boost::algorithm::to_lower_copy( shortname_ );
    if( name_.empty() || shortname_.empty() )
        throw MASA_EXCEPTION( QString( "Incomplete language '%1' / '%2'" ).arg( name_.c_str() ).arg( shortname_.c_str() ).toStdString() );
    else if( shortname_.size() != 2 )
        throw MASA_EXCEPTION( QString( "Invalid language shortname '%1'" ).arg( shortname_.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: Language::Write
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void Language::Write( xml::xostream& xos ) const
{
    xos << xml::attribute( "name", name_ )
        << xml::attribute( "shortname", shortname_ );
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
// Name: Language::GetShortName
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
const std::string& Language::GetShortName() const
{
    return shortname_;
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
