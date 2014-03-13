// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __MIL_AgentTypePionRemote_h_
#define __MIL_AgentTypePionRemote_h_

#include "Entities/Agents/MIL_AgentTypePion.h"

class MIL_AgentTypePion_Remote : public MIL_AgentTypePion
{
public:
    MIL_AgentTypePion_Remote( const std::string& strName, const std::string& strType, xml::xistream& xis );
    virtual ~MIL_AgentTypePion_Remote();

    static const MIL_AgentTypePion* Create( const std::string& strName, const std::string& strType, xml::xistream& xis );

    //! @name Instantiation
    //@{
    virtual MIL_AgentPion* InstanciatePion( MissionController_ABC& controller, MIL_Automate& automate, xml::xistream& xis ) const;

    virtual void RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const;
    //@}
};

#endif // __MIL_AgentTypePionRemote_h_
