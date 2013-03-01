// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_HELPERS_H
#define SWORD_HELPERS_H

#define GET_DATA( node, data ) (*core::Convert( node ))[ "data" ].GetUserData< data >()
#define GET_PION( node ) GET_DATA( node, MIL_AgentPion )
#define GET_ROLE( node, role ) GET_PION( node ).GetRole< role >()
#define GET_CHILD_DATA( node, child, data ) (*core::Convert( node ))[ child "/data" ].GetUserData< data >()

#endif // SWORD_HELPERS_H
