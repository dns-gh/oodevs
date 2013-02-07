// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "MIL_AgentPion_Remote.h"


MIL_AgentPion_Remote::MIL_AgentPion_Remote( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis )
    : MIL_AgentPion( type, automate, algorithmFactories, xis )
{
    // NOTHING
}

MIL_AgentPion_Remote::MIL_AgentPion_Remote( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name )
    : MIL_AgentPion( type, automate, algorithmFactories, name )
{
    // NOTHING
}

MIL_AgentPion_Remote::~MIL_AgentPion_Remote()
{
    // NOTHING
}

