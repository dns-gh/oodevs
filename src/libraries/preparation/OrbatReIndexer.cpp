// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "OrbatReIndexer.h"
#include "IdManager.h"
#include "OrbatImportFilter.h"
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: OrbatReIndexer constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
OrbatReIndexer::OrbatReIndexer( xml::xistream& input, xml::xostream& output, IdManager& idManager, const OrbatImportFilter& filter )
    : shift_( idManager.GetNextId() )
    , output_( output )
{
    output_ << xml::start( "orbat" );
    input   >> xml::start( "orbat" )
                >> xml::list( *this, &OrbatReIndexer::ReadField, filter )
            >> xml::end();
    output_ << xml::end();
}

// -----------------------------------------------------------------------------
// Name: OrbatReIndexer destructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
OrbatReIndexer::~OrbatReIndexer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrbatReIndexer::ReadField
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void OrbatReIndexer::ReadField( const std::string& name, xml::xistream& xis, const OrbatImportFilter& filter )
{
    if( filter.Filters( name ) )
        return;
    output_ << xml::start( name );
    std::string text;
    xis >> xml::attributes( *this, &OrbatReIndexer::ReadAttribute )
            >> xml::list( *this, &OrbatReIndexer::ReadField, filter )
            >> xml::optional() >> text;
    // $$$$ SBO 2008-04-08: check for cdata
    if( !text.empty() )
        output_ << text;
    output_ << xml::end();
}

namespace
{
    bool IsIndex( const std::string& attribute )
    {
        return attribute == "id"
            || attribute == "knowledge-group"
            || attribute == "side";
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatReIndexer::ReadAttribute
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void OrbatReIndexer::ReadAttribute( const std::string& name, xml::xistream& xis )
{
    if( IsIndex( name ) )
        output_ << xml::attribute( name, xml::attribute< unsigned long >( xis, name ) + shift_ );
    else
        output_ << xml::attribute( name, xml::attribute< std::string >( xis, name ) );
}
