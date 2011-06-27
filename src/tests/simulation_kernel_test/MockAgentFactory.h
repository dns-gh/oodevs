// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockAgentFactory_h_
#define __MockAgentFactory_h_

#include "simulation_kernel/AgentFactory_ABC.h"

MOCK_BASE_CLASS( MockAgentFactory, AgentFactory_ABC )
{
    MOCK_METHOD_EXT( Create, 3, MIL_AgentPion*( const MIL_AgentTypePion&, MIL_Automate&, xml::xistream& ), CreateFromXml );
    MOCK_METHOD_EXT( Create, 3, MIL_AgentPion*( const MIL_AgentTypePion&, MIL_Automate&, const MT_Vector2D& ), CreateFromPoint );
    MOCK_METHOD_EXT( Create, 4, MIL_AgentPion*( const MIL_AgentTypePion&, MIL_Automate&, const MT_Vector2D&, const std::string& ), CreateFromPointAndName );
};

#endif // __MockArmy_h_
