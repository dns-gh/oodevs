// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h $
// $Author: Jvt $
// $Modtime: 31/03/05 16:42 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Humans.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Humans_h_
#define __PHY_RoleInterface_Humans_h_

#include "MT_Tools/Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Humans
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Humans : public tools::Role_ABC
                               , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Humans RoleInterface;
    //@}

public:
             PHY_RoleInterface_Humans();
    virtual ~PHY_RoleInterface_Humans();
};

#endif // __PHY_RoleInterface_Humans_h_
