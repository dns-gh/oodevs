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

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Medical
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Medical : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Medical )

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Medical RoleInterface;
    //@}

public:
             PHY_RoleInterface_Medical( MT_RoleContainer& role );
             PHY_RoleInterface_Medical();
    virtual ~PHY_RoleInterface_Medical();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Medical::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Medical::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Medical_h_
