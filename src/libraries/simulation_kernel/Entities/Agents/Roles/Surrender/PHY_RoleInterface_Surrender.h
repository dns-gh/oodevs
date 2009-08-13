// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h $
// $Author: Nld $
// $Modtime: 13/05/05 10:11 $
// $Revision: 4 $
// $Workfile: PHY_RoleInterface_Surrender.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Surrender_h_
#define __PHY_RoleInterface_Surrender_h_

#include "MT_Tools/Role_ABC.h"

class MIL_AgentPion;
class MIL_Object_ABC;
class MIL_Army;

// =============================================================================
// @class  PHY_RoleInterface_Surrender
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Surrender : public tools::Role_ABC
                                  , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Surrender RoleInterface;

public:
             PHY_RoleInterface_Surrender();
    virtual ~PHY_RoleInterface_Surrender();
    
    //! @name Accessors
    //@{
    virtual const MIL_Army* GetArmySurrenderedTo() const = 0;
    virtual       bool      IsSurrendered       () const = 0;
    virtual       bool      IsPrisoner          () const = 0;
    virtual       bool      IsImprisoned        ( const MIL_Object_ABC& camp ) = 0;
    //@}

    //! @name Operations
    //@{
    virtual bool Capture ( const MIL_AgentPion& pionTakingPrisoner ) = 0;
    virtual bool Release () = 0;
    virtual bool Imprison( const MIL_Object_ABC& camp ) = 0;
    //@}
};

#endif // __PHY_RoleInterface_Surrender_h_
