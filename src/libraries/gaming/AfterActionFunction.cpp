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
#include <xeumeuleu/xml.h>

using namespace kernel;

namespace 
{
    std::string ReadBase( xml::xistream& xis )
    {
        xml::xostringstream xos;
        xos << xml::start( "indicator" );
        xis >> xml::start( "indicator" )
                >> xos
            >> xml::end();
        xos << xml::end();
        return xos.str();
    }
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunction constructor
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
AfterActionFunction::AfterActionFunction( xml::xistream& xis )
    : name_( xml::attribute< std::string >( xis, "name" ).c_str() )
    , base_( ReadBase( xis ) )
{
    std::string comments;
    xis >> xml::content( "comments", comments )
        >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &AfterActionFunction::ReadParameter )
        >> xml::end();
    comments_ = comments.c_str();
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
// Name: AfterActionFunction::ReadParameter
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionFunction::ReadParameter( xml::xistream& xis )
{
    const std::string name = xml::attribute< std::string >( xis, "name" );
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
    for( T_Parameters::const_iterator it = parameters.begin(); it != parameters.end(); ++it )
        Get( it->first ).Commit( result, it->second );
    return result;
}
