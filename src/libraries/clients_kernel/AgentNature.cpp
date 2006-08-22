// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AgentNature.h"
#include "xeumeuleu/xml.h"
#include <boost/algorithm/string.hpp>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: AgentNature constructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
AgentNature::AgentNature( xml::xistream& xis )
{
    xis >> start( "Nature" );
    ReadNature( xis, "Niveau", "level" );
    ReadNature( xis, "Arme", "weapon" );
    ReadNature( xis, "Specialisation", "specialization" );
    ReadNature( xis, "Qualificatif", "qualifier" );
    ReadNature( xis, "Categorie", "category" );
    ReadNature( xis, "TypeMobilite", "mobility" );
    ReadNature( xis, "QualificatifAtlas", "atlas qualifier" );
    ReadNature( xis, "CapaciteMission", "mission capacity" );
    xis >> end();
}
    
// -----------------------------------------------------------------------------
// Name: AgentNature destructor
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
AgentNature::~AgentNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentNature::ReadNature
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void AgentNature::ReadNature( xml::xistream& xis, const std::string& name, const std::string& symbolName )
{
    std::string type;
    xis >> start( name )
            >> attribute( "type", type )
        >> end();
    boost::replace_all( type, ",", "" );
    boost::to_lower( type );
    natures_[ symbolName ] = type;
}

// -----------------------------------------------------------------------------
// Name: AgentNature::Retrieve
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
const std::string& AgentNature::Retrieve( const std::string& symbolName ) const
{
    CIT_Natures it = natures_.find( symbolName );
    if( it != natures_.end() )
        return it->second;
    throw std::runtime_error( "Trying to retrieve an unknown nature field: " + symbolName );
}
