// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "orbat_generator_pch.h"
#include "Pion.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <xeumeuleu/xml.hpp>

using namespace orbat_generator;

// -----------------------------------------------------------------------------
// Name: Pion constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Pion::Pion( bool posteDeCommandement, const std::string type )
    : pc_   ( posteDeCommandement )
    , type_ ( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Pion destructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Pion::~Pion()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Pion::InsertIntoOrbat
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Pion::InsertIntoOrbat( xml::xostream& orbat, const IdNameGenerator& idNameGen )
{
    unsigned int id = idNameGen.ComputePionId();
    std::string name = idNameGen.ComputePionName();
    orbat << xml::start( "unit" )
            << xml::attribute< bool >( "command-post", pc_ )
            << xml::attribute< int >( "id", id )
            << xml::attribute< std::string >( "name", name )
            << xml::attribute< std::string >( "position", idNameGen.ComputeCoord() )
            << xml::attribute< std::string >( "type", type_ )
          << xml::end;
}
