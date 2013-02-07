// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef SWORD_ROLEEXTENDER_ABC_H_
#define SWORD_ROLEEXTENDER_ABC_H_

class MIL_AgentPion;

struct RoleExtender_ABC
{
    virtual ~RoleExtender_ABC() {}
    virtual void RegisterRoles( MIL_AgentPion& ) = 0;
};


#endif // SWORD_ROLEEXTENDER_ABC_H_
