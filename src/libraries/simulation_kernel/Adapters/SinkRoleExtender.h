// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef SWORD_SINKROLEEXTENDER_H
#define SWORD_SINKROLEEXTENDER_H

#include "Entities/Agents/RoleExtender_ABC.h"

/// =============================================================================
/// @class SinkRoleExtender
/// @brief SinkRoleExtender
///
/// Created: ahc 7 févr. 2013
/// =============================================================================
class SinkRoleExtender : public RoleExtender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SinkRoleExtender( RoleExtender_ABC* chain, boost::function< MIL_AgentPion&(MIL_AgentPion&) > configure );
    //@}

    virtual void RegisterRoles( MIL_AgentPion& pion );

private:
    //! @name Member data
    //@{
    RoleExtender_ABC* chain_;
    boost::function< MIL_AgentPion&(MIL_AgentPion&) > configure_;
    //@}
};

#endif // SWORD_SINKROLEEXTENDER_H
