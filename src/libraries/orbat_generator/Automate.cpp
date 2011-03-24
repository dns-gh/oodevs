// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "Automate.h"
#include <iostream>
#include <xeumeuleu/xml.hpp>

using namespace orbat_generator;

// -----------------------------------------------------------------------------
// Name: Automate constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Automate::Automate( std::string type, std::vector< boost::shared_ptr< Pion > > pions, const std::string& odbType )
    : type_   ( type )
    , pions_  ( pions )
    , engaged_( odbType == "automats" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automate destructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Automate::~Automate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::InsertIntoOrbat
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Automate::InsertIntoOrbat( xml::xostream& orbat, IdNameGenerator& idNameGen, unsigned int KnowledgeGroupId )
{
    unsigned int id = idNameGen.ComputeAutomateId();
    std::string name = idNameGen.ComputeAutomateName();
    orbat << xml::start( "automat" )
            << xml::attribute< bool >( "engaged", engaged_ )
            << xml::attribute< int >( "id", id )
            << xml::attribute< int >( "knowledge-group", KnowledgeGroupId )
            << xml::attribute< std::string >( "name", name )
            << xml::attribute< std::string >( "type", type_ );
    for ( unsigned int it = 0; it < pions_.size(); ++it )
        pions_[ it ]->InsertIntoOrbat( orbat, idNameGen );
    orbat << xml::end;
}
