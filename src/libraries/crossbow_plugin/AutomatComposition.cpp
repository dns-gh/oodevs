// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "AutomatComposition.h"
#include "Resolver_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatComposition constructor
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
AutomatComposition::AutomatComposition( xml::xistream& input, const Resolver_ABC< AgentType, std::string >& agentResolver )
    : type_ ( agentResolver.Get( ReadType( input ) ) )     
{
    // NOTHING    
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition destructor
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
AutomatComposition::~AutomatComposition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::ReadType
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
std::string AutomatComposition::ReadType( xml::xistream& input )
{
    std::string name;
    input >> xml::attribute( "nom", name );
    return name;
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetType
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
const AgentType& AutomatComposition::GetType() const
{
    return type_;
}
