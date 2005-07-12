// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h $
// $Author: Jvt $
// $Modtime: 31/03/05 11:11 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Transported.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Transported_h_
#define __PHY_RoleInterface_Transported_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Transported
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Transported : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Transported )

public:
    typedef PHY_RoleInterface_Transported RoleInterface;

public:
             PHY_RoleInterface_Transported( MT_RoleContainer& role );
             PHY_RoleInterface_Transported();
    virtual ~PHY_RoleInterface_Transported();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

#include "PHY_RoleInterface_Transported.inl"

#endif // __PHY_RoleInterface_Transported_h_
