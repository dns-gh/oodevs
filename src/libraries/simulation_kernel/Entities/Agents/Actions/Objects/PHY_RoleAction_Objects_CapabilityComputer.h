// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects_CapabilityComputer.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:03 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_Objects_CapabilityComputer.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_Objects_CapabilityComputer_h_
#define __PHY_RoleAction_Objects_CapabilityComputer_h_

#include "MIL.h"

class MIL_RealObjectType;
class PHY_ComposantePion;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RoleAction_Objects_CapabilityComputer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects_CapabilityComputer
{
    MT_COPYNOTALLOWED( PHY_RoleAction_Objects_CapabilityComputer )

public:
    //! @name Types
    //@{
    enum E_Operation
    {
        eConstruct,
        eDestroy,
        eMine,
        eDemine,
        eBypass,
        ePrepare
    };
    //@}

public:
     PHY_RoleAction_Objects_CapabilityComputer( const MIL_AgentPion& pion, E_Operation nOperation, const MIL_RealObjectType& objectType );
    ~PHY_RoleAction_Objects_CapabilityComputer();

    //! @name Operations
    //@{
    bool HasCapability() const;
    void operator()   ( const PHY_ComposantePion& composante );
    //@}

private:
    //! @name Operations
    //@{
    void CollectData( const MIL_AgentPion& pion );
    //@}

private:
    const E_Operation         nOperation_;
    const MIL_RealObjectType& objectType_;
          bool                bHasCapability_;
};

#include "PHY_RoleAction_Objects_CapabilityComputer.inl"

#endif // __PHY_RoleAction_Objects_CapabilityComputer_h_
