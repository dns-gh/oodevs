// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RoleInterface_Supply.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:14 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Supply.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Supply_h_
#define __PHY_RoleInterface_Supply_h_

#include "MT_Tools/Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Supply : public tools::Role_ABC
                               , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Supply RoleInterface;
    //@}

public:
             PHY_RoleInterface_Supply();
    virtual ~PHY_RoleInterface_Supply();
};

#endif // __PHY_RoleInterface_Supply_h_
