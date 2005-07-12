// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RoleHLA_Surrender.h $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 2 $
// $Workfile: PHY_RoleHLA_Surrender.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Surrender_h_
#define __PHY_RoleHLA_Surrender_h_

#include "MIL.h"

#include "PHY_RoleInterface_Surrender.h"

// =============================================================================
// @class  PHY_RoleHLA_Surrender
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleHLA_Surrender : public PHY_RoleInterface_Surrender
{
    MT_COPYNOTALLOWED( PHY_RoleHLA_Surrender )

public:
    PHY_RoleHLA_Surrender( MT_RoleContainer& role );
    virtual ~PHY_RoleHLA_Surrender();

    //! @name Main
    //@{
    virtual bool TakePrisoner             ( const MIL_AgentPion& pionTakingPrisoner );
    virtual void NotifyInsidePrisonerCamp ( const MIL_CampPrisonniers& camp );
    virtual void NotifyOutsidePrisonerCamp( const MIL_CampPrisonniers& camp );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsSurrendered() const;
    virtual bool IsPrisoner   () const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    bool bSurrendered_;
    bool bPrisoner_;
};

#include "PHY_RoleHLA_Surrender.inl"

#endif // __PHY_RoleHLA_Surrender_h_
