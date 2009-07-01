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

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Supply
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Supply : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Supply )

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Supply RoleInterface;
    //@}

public:
    explicit PHY_RoleInterface_Supply( MT_RoleContainer& role );
             PHY_RoleInterface_Supply();
    virtual ~PHY_RoleInterface_Supply();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Supply::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Supply::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Supply_h_
