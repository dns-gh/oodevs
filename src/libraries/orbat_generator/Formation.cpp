// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "orbat_generator_pch.h"
#include "Formation.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <xeumeuleu/xml.hpp>

using namespace orbat_generator;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Formation::Formation( std::string type )
    : type_   ( type )
    , current_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::InsertIntoOrbat
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Formation::InsertIntoOrbat( xml::xostream& orbat, const IdNameGenerator& idNameGen )
{
    unsigned int id = idNameGen.ComputeFormationId();
    unsigned int knowledgeId = idNameGen.ComputeKnowledgeId();
    std::string name = idNameGen.ComputeFormationName();
    orbat << xml::start( "formation" )
            << xml::attribute< int >( "id", id )
            << xml::attribute< std::string >( "level", type_ )
            << xml::attribute< std::string >( "name", name );
    for ( unsigned int it = 0; it < automates_.size(); ++it )
        automates_[ it ]->InsertIntoOrbat( orbat, idNameGen, knowledgeId * 1000 );
    orbat << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Formation::AddAutomate
// Created: RCD 2011-03-04
// -----------------------------------------------------------------------------
void Formation::AddAutomate( boost::shared_ptr< Automate > automate )
{
    automates_.push_back( automate );
}
