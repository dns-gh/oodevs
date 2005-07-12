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

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Maintenance
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Maintenance : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Maintenance )

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Maintenance RoleInterface;
    //@}

public:
             PHY_RoleInterface_Maintenance( MT_RoleContainer& role );
             PHY_RoleInterface_Maintenance();
    virtual ~PHY_RoleInterface_Maintenance();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

#include "PHY_RoleInterface_Maintenance.inl"

#endif // __PHY_RoleInterface_Maintenance_h_
