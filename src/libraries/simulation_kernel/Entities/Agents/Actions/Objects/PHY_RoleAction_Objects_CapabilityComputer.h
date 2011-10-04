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

#include "OnComponentFunctor_ABC.h"
#include "Operation.h"

class MIL_ObjectType_ABC;
class PHY_ComposantePion;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RoleAction_Objects_CapabilityComputer
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_Objects_CapabilityComputer : public OnComponentFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleAction_Objects_CapabilityComputer( const MIL_AgentPion& pion, E_Operation nOperation, const MIL_ObjectType_ABC& objectType,
                                                        bool bWithLoaded, bool bWithReinforcement = true );
    virtual ~PHY_RoleAction_Objects_CapabilityComputer();
    //@}

    //! @name Operations
    //@{
    bool HasCapability() const;
    void operator()( PHY_ComposantePion& composante );
    //@}

private:
    //! @name Operations
    //@{
    void CollectData( const MIL_AgentPion& pion, bool bWithReinforcement );
    //@}

private:
    //! @name Member data
    //@{
    const E_Operation nOperation_;
    const MIL_ObjectType_ABC& objectType_;
    bool bHasCapability_;
    bool bWithLoaded_;
    //@}
};

#endif // __PHY_RoleAction_Objects_CapabilityComputer_h_
