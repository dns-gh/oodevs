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

#include "MIL.h"

#include "MIL_Effect_ABC.h"

class PHY_Composante_ABC;
class PHY_DotationCategory_IndirectFire_ABC;
class PHY_WeaponDataType_IndirectFire;
class PHY_FireResults_Pion;
class PHY_Weapon;
class MIL_AgentPion;

// =============================================================================
// @class  MIL_Effect_IndirectFire
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_IndirectFire : public MIL_Effect_ABC
{
    MT_COPYNOTALLOWED( MIL_Effect_IndirectFire )

public:
             MIL_Effect_IndirectFire( const MIL_AgentPion& firer, uint nTargetKnowledgeID           , const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory, MT_Float rInterventionTypeToFire );
             MIL_Effect_IndirectFire( const MIL_AgentPion& firer, const MT_Vector2D& vTargetPosition, const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory, MT_Float rInterventionTypeToFire );
    virtual ~MIL_Effect_IndirectFire();

    //! @name Accessors
    //@{
          bool                                   IsInterventionTypeFired    () const;
          bool                                   IsTargetValid              () const;

    const PHY_DotationCategory_IndirectFire_ABC& GetIndirectDotationCategory         () const;
          uint                                   GetFireID                           () const;
          uint                                   GetNbrAmmoFired                     () const;
          uint                                   GetNbrAmmoToCompleteInterventionType() const;

    void IncRef();
    void DecRef();
    //@}

    //! @name Operations
    //@{
    virtual bool Execute();

    void ForceFlying                (); 
    void NotifyAmmoFired            ( const PHY_WeaponDataType_IndirectFire& weaponType, uint nNbrAmmoReserved );
    bool IsFlyingThroughLocalisation( const TER_Localisation& localisation ) const;
    bool CanWeaponBeUsed            ( const PHY_Weapon& weapon ) const;
    //@}

private:
    //! @name Tools
    //@{
    void UpdateTargetPositionFromKnowledge();
    void StartFlying                      ();
    void StopFlying                       (); 
    //@}

private:
          uint                                   nNbrRefs_;
    const MIL_AgentPion&                         firer_;
    const MT_Float                               rInterventionTypeToFire_;
    const PHY_DotationCategory_IndirectFire_ABC& indirectDotationCategory_;                                                
    const MT_Vector2D                            vSourcePosition_;
          MT_Vector2D                            vTargetPosition_;
          uint                                   nTargetKnowledgeID_;
          uint                                   nNbrAmmoFired_;
          bool                                   bIsFlying_;
          bool                                   bFired_;
          MT_Float                               rImpactTimeStep_;
          PHY_FireResults_Pion*                  pFireResult_;
};

#include "MIL_Effect_IndirectFire.inl"

#endif // __MIL_Effect_IndirectFire_h_
