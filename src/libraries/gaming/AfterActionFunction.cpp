// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionFunction.h"
#include "AfterActionParameter.h"
#include "clients_kernel/Tools.h"
#include "tools/Language.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    std::string ReadBase( xml::xistream& xis )
    {
        xml::xostringstream xos;
        xos << xml::start( "indicator" );
        xis >> xml::start( "indicator" )
                >> xos
            >> xml::end;
        xos << xml::end;
        return xos.str();
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction constructor
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
AfterActionFunction::AfterActionFunction( xml::xistream& xis )
    : base_( ReadBase( xis ) )
{
    xis >> xml::start( "descriptions" )
          >> xml::list( "description", *this, &AfterActionFunction::ReadDescription )
        >> xml::end;
    if( name_.isEmpty() )
        name_ = xis.attribute< std::string >( "name" ).c_str();
    xis >> xml::start( "parameters" )
          >> xml::list( "parameter", *this, &AfterActionFunction::ReadParameter )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction destructor
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionFunction::~AfterActionFunction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::ReadDescription
// Created: PHC 2011-02-15
// -----------------------------------------------------------------------------
void AfterActionFunction::ReadDescription( xml::xistream& xis )
{
    if( xis.attribute< std::string >( "lang" ) == tools::Language::Current() || ( name_.isEmpty() && xis.attribute< std::string >( "lang" ) == "en" ) )
    {
        std::string comments;
        xis >> comments;
        name_ = xis.attribute< std::string >( "name" ).c_str();
        comments_ = comments.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::ReadParameter
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunction::ReadParameter( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name" );
    Register( name, *new AfterActionParameter( xis ) );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::GetName
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
QString AfterActionFunction::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::GetComments
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
QString AfterActionFunction::GetComments() const
{
    return comments_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction::Commit
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
std::string AfterActionFunction::Commit( const T_Parameters& parameters ) const
{
    std::string result( base_ );
    for( auto it = parameters.begin(); it != parameters.end(); ++it )
        Get( it->first ).Commit( result, it->second );
    return result;
}
