// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TranslationQuery.h"
#include "XQuery.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TranslationQuery constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
TranslationQuery::TranslationQuery()
    : query_()
    , context_()
    , prefix_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
TranslationQuery::TranslationQuery( const TranslationQuery& other )
    : query_( other.query_ )
    , context_( other.context_ )
    , prefix_( other.prefix_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery constructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
TranslationQuery::TranslationQuery( xml::xistream& xis )
    : query_()
    , context_()
    , prefix_()
{
    Read( xis );
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery destructor
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
TranslationQuery::~TranslationQuery()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery::Read
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void TranslationQuery::Read( xml::xistream& xis )
{
    xis >> xml::attribute( "query", query_ )
        >> xml::attribute( "context", context_ )
        >> xml::optional >> xml::attribute( "prefix", prefix_ );
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery::Write
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
void TranslationQuery::Write( xml::xostream& xos ) const
{
    xos << xml::attribute( "query", query_ )
        << xml::attribute( "context", context_ );
    if( !prefix_.empty() )
        xos << xml::attribute( "prefix", prefix_ );
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery::GetContext
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
const std::string& TranslationQuery::GetContext() const
{
    return context_;
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery::Evaluate
// Created: ABR 2013-07-09
// -----------------------------------------------------------------------------
QStringList TranslationQuery::Evaluate( const tools::Path& path ) const
{
    return kernel::xquery::Evaluate( path, query_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery::operator>>
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
xml::xistream& kernel::operator>>( xml::xistream& xis, TranslationQuery& query )
{
    xis >> xml::start( "translation-query" );
    query.Read( xis );
    xis >> xml::end;
    return xis;
}

// -----------------------------------------------------------------------------
// Name: TranslationQuery::operator<<
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
xml::xostream& kernel::operator<<( xml::xostream& xos, const TranslationQuery& query )
{
    xos << xml::start( "translation-query" );
    query.Write( xos );
    xos << xml::end;
    return xos;
}
