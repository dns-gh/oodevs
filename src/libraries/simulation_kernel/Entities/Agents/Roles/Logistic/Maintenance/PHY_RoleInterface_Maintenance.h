// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_RoleInterface_Maintenance.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:17 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Maintenance.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Maintenance_h_
#define __PHY_RoleInterface_Maintenance_h_

#include "MT_Tools/Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Maintenance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Maintenance : public tools::Role_ABC
                                    , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Maintenance RoleInterface;
    //@}

public:
             PHY_RoleInterface_Maintenance();
    virtual ~PHY_RoleInterface_Maintenance();
};

#endif // __PHY_RoleInterface_Maintenance_h_
