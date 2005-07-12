// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h $
// $Author: Nld $
// $Modtime: 2/05/05 18:25 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Perceiver.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Perceiver_h_
#define __PHY_RoleInterface_Perceiver_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Perceiver
// Created: JVT 2004-08-03
// Modified: JVT 2004-10-28
// =============================================================================
class PHY_RoleInterface_Perceiver : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Perceiver )

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Perceiver RoleInterface;
    //@}

public:
             PHY_RoleInterface_Perceiver( MT_RoleContainer& role );
             PHY_RoleInterface_Perceiver();
    virtual ~PHY_RoleInterface_Perceiver();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{    
    virtual bool IsUsingActiveRadar() const = 0;
    //@}
};

#include "PHY_RoleInterface_Perceiver.inl"

#endif // __PHY_RoleInterface_Perceiver_h_
