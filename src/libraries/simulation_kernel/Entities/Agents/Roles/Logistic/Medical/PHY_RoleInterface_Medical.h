// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_RoleInterface_Medical.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:40 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Medical.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Medical_h_
#define __PHY_RoleInterface_Medical_h_

#include "MT_Tools/Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Medical
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Medical : public tools::Role_ABC
                                , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Medical RoleInterface;
    //@}

public:
             PHY_RoleInterface_Medical();
    virtual ~PHY_RoleInterface_Medical();
};

#endif // __PHY_RoleInterface_Medical_h_
