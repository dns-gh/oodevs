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

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Reinforcement
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Reinforcement : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Reinforcement )

public:
    typedef PHY_RoleInterface_Reinforcement RoleInterface;

public:
             PHY_RoleInterface_Reinforcement( MT_RoleContainer& role );
             PHY_RoleInterface_Reinforcement();
    virtual ~PHY_RoleInterface_Reinforcement();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Reinforcement::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Reinforcement::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Reinforcement_h_
