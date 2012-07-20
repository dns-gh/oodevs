// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "NBCAgent.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: NBCAgent constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
NBCAgent::NBCAgent( xml::xistream& xis )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "id", id_ )
        >> xml::list( "effects", *this, &NBCAgent::ReadEffect );
}

// -----------------------------------------------------------------------------
// Name: NBCAgent destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
NBCAgent::~NBCAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCAgent::GetId
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
unsigned long NBCAgent::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: NBCAgent::GetName
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
std::string NBCAgent::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: NBCAgent::ReadEffect
// Created: NPT 2012-07-20
// -----------------------------------------------------------------------------
void NBCAgent::ReadEffect( xml::xistream& xis )
{
    std::string attribute; 
    attribute = xis.attribute< std::string >( "type", "" );
    
    if( attribute == "liquid" )
        effectTypes_.push_back( eLiquid );
    else if( attribute == "gaseous" )
        effectTypes_.push_back( eGazeous );
    else
        throw std::exception( __FUNCTION__ "invalid effect type" );
}

// -----------------------------------------------------------------------------
// Name: std::vector< E_EffectType > NBCAgent::GetEffects
// Created: NPT 2012-07-20
// -----------------------------------------------------------------------------
const NBCAgent::T_Effects& NBCAgent::GetEffects() const
{
    return effectTypes_ ;
}


