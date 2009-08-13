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

#include "PHY_RoleInterface_Surrender.h"

// =============================================================================
// @class  PHY_RoleHLA_Surrender
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleHLA_Surrender : public PHY_RoleInterface_Surrender
{

public:
             PHY_RoleHLA_Surrender();
    virtual ~PHY_RoleHLA_Surrender();

    //! @name Main
    //@{
    virtual bool Capture( const MIL_AgentPion& pionTakingPrisoner );
    virtual bool Release();
    virtual bool Imprison( const MIL_Object_ABC& camp );
    //@}

    //! @name Accessors
    //@{
    virtual const MIL_Army* GetArmySurrenderedTo() const;
    virtual       bool      IsSurrendered       () const;
    virtual       bool      IsPrisoner          () const;
    virtual       bool      IsImprisoned        ( const MIL_Object_ABC& camp );
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    bool bPrisoner_;
};

#endif // __PHY_RoleHLA_Surrender_h_
