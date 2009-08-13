// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:20 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Reinforcement.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Reinforcement_h_
#define __PHY_RoleInterface_Reinforcement_h_

#include "MT_Tools/Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Reinforcement
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Reinforcement : public tools::Role_ABC
                                      , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Reinforcement RoleInterface;

public:
             PHY_RoleInterface_Reinforcement();
    virtual ~PHY_RoleInterface_Reinforcement();
};

#endif // __PHY_RoleInterface_Reinforcement_h_
