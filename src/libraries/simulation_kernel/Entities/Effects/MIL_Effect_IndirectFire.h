// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_IndirectFire.h $
// $Author: Jvt $
// $Modtime: 2/05/05 17:09 $
// $Revision: 6 $
// $Workfile: MIL_Effect_IndirectFire.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_IndirectFire_h_
#define __MIL_Effect_IndirectFire_h_

#include "MIL_Effect_ABC.h"
#include "MT_Tools/MT_Vector2D.h"
#include <boost/optional.hpp>

class PHY_DotationCategory_IndirectFire_ABC;
class PHY_WeaponDataType_IndirectFire;
class PHY_FireResults_Pion;
class PHY_Weapon;
class MIL_Agent_ABC;
class TER_Localisation;

// =============================================================================
// @class  MIL_Effect_IndirectFire
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_IndirectFire : public MIL_Effect_ABC
{
public:
             MIL_Effect_IndirectFire( const MIL_Agent_ABC& firer, unsigned int nTargetKnowledgeID, const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory, double rInterventionTypeToFire );
             MIL_Effect_IndirectFire( const MIL_Agent_ABC& firer, const MT_Vector2D& vTargetPosition, const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory, double rInterventionTypeToFire );
    virtual ~MIL_Effect_IndirectFire();

    //! @name Accessors
    //@{
    bool IsInterventionTypeFired() const;
    bool MustWaitImpact() const;
    bool IsTargetValid() const;
    const PHY_DotationCategory_IndirectFire_ABC& GetIndirectDotationCategory() const;
    unsigned int GetFireID() const;
    unsigned int GetNbrAmmoFired() const;
    unsigned int GetNbrAmmoToCompleteInterventionType() const;
    const MIL_Agent_ABC& GetFirer() const;

    void IncRef();
    void DecRef();
    //@}

    //! @name Operations
    //@{
    virtual bool Execute();

    void StartFlying();
    void NotifyAmmoFired( const PHY_WeaponDataType_IndirectFire& weaponType, unsigned int nNbrAmmoReserved );
    bool IsFlyingThroughLocalisation( const TER_Localisation& localisation ) const;
    bool CanWeaponBeUsed( const PHY_Weapon& weapon ) const;
    //@}

private:
    //! @name Tools
    //@{
    void UpdateTargetPositionFromKnowledge();
    void StopFlying();
    //@}

private:
    unsigned int nNbrRefs_;
    const MIL_Agent_ABC& firer_;
    const double rInterventionTypeToFire_;
    const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory_;
    const MT_Vector2D vSourcePosition_;
    boost::optional< MT_Vector2D > vTargetPosition_;
    unsigned int nTargetKnowledgeID_;
    unsigned int nNbrAmmoFired_;
    unsigned int fireResultId_;
    bool bIsFlying_;
    bool bFired_;
    bool bArrived_;
    double rImpactTimeStep_;
    std::unique_ptr< PHY_FireResults_Pion > pFireResult_;
};

#endif // __MIL_Effect_IndirectFire_h_
