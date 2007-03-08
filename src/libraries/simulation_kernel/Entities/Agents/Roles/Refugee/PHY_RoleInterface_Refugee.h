// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:10 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Refugee.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Refugee_h_
#define __PHY_RoleInterface_Refugee_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

class MIL_CampRefugies;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RoleInterface_Refugee
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Refugee : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Refugee )

public:
    typedef PHY_RoleInterface_Refugee RoleInterface;

public:
             PHY_RoleInterface_Refugee( MT_RoleContainer& role );
             PHY_RoleInterface_Refugee();
    virtual ~PHY_RoleInterface_Refugee();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Accessors
    //@{
    virtual bool IsManaged() const = 0;
    virtual bool IsManaged( const MIL_CampRefugies& camp ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual bool Orientate( const MIL_AgentPion& pionManaging ) = 0;
    virtual bool Release  () = 0;
    virtual bool Release  ( const MIL_CampRefugies& camp ) = 0;
    //@}
};

#include "PHY_RoleInterface_Refugee.inl"

#endif // __PHY_RoleInterface_Refugee_h_
