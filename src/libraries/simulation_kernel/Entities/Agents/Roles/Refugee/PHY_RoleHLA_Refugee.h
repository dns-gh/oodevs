// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Refugee/PHY_RoleHLA_Refugee.h $
// $Author: Nld $
// $Modtime: 10/03/05 17:58 $
// $Revision: 1 $
// $Workfile: PHY_RoleHLA_Refugee.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Refugee_h_
#define __PHY_RoleHLA_Refugee_h_

#include "MIL.h"

#include "PHY_RoleInterface_Refugee.h"

// =============================================================================
// @class  PHY_RoleHLA_Refugee
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleHLA_Refugee : public PHY_RoleInterface_Refugee
{
    MT_COPYNOTALLOWED( PHY_RoleHLA_Refugee )

public:
    PHY_RoleHLA_Refugee( MT_RoleContainer& role );
    virtual ~PHY_RoleHLA_Refugee();

    //! @name Accessors
    //@{
    virtual bool IsManaged() const;
    virtual bool IsManaged( const MIL_CampRefugies& camp ) const;
    //@}

    //! @name Operations
    //@{
    virtual bool Orientate( const MIL_AgentPion& pionManaging );
    virtual bool Release  ();
    virtual bool Release  ( const MIL_CampRefugies& camp );
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    bool bManaged_;
};

#include "PHY_RoleHLA_Refugee.inl"

#endif // __PHY_RoleHLA_Refugee_h_
