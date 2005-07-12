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

class PHY_WeaponDataType_IndirectFire;
class PHY_Composante_ABC;
class PHY_IndirectFireDotationClass;
class MIL_AgentPion;
class PHY_Weapon;
class PHY_DotationCategory;
class PHY_WeaponDataType_IndirectFire;
class PHY_IndirectFireResults;

// =============================================================================
// @class  MIL_Effect_IndirectFire
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_IndirectFire : public MIL_Effect_ABC
{
    MT_COPYNOTALLOWED( MIL_Effect_IndirectFire )

public:
             MIL_Effect_IndirectFire( const MIL_AgentPion& firer, uint nTargetKnowledgeID           , const PHY_IndirectFireDotationClass& indirectWeaponClass, MT_Float rInterventionTypeToFire );
             MIL_Effect_IndirectFire( const MIL_AgentPion& firer, const MT_Vector2D& vTargetPosition, const PHY_IndirectFireDotationClass& indirectWeaponClass, MT_Float rInterventionTypeToFire );
    virtual ~MIL_Effect_IndirectFire();

    //! @name Accessors
    //@{
    bool IsInterventionTypeFired() const;
    bool IsTargetValid          () const;

    void IncRef();
    void DecRef();
    //@}

    //! @name Operations
    //@{
    virtual bool Execute();

          uint                  GetFireID                           () const;
          void                  ForceFlying                         (); 
          bool                  CanWeaponBeUsed                     ( const PHY_Weapon& weapon ) const;
    const PHY_DotationCategory* GetWeaponDotationCategory           () const;
          void                  SetWeaponDotationCategory           ( const PHY_DotationCategory& dotationCategory );
          void                  NotifyAmmoFired                     ( const PHY_WeaponDataType_IndirectFire& weaponType, uint nNbrAmmoReserved );
          uint                  GetNbrAmmoFired                     () const;
          uint                  GetNbrAmmoToCompleteInterventionType() const;
          bool                  FlyThroughLocalisation              ( const TER_Localisation& localisation ) const;
          MT_Float              GetFlyingDistance                   () const;
    //@}

private:
    //! @name Tools
    //@{
    void UpdateTargetPositionFromKnowledge();
    //@}

private:
          uint                           nNbrRefs_;
    const MIL_AgentPion&                 firer_;
    const PHY_IndirectFireDotationClass& indirectWeaponClass_;
    const MT_Float                       rInterventionTypeToFire_;
    const PHY_DotationCategory*          pWeaponDotationCategory_;
    const MT_Vector2D                    vSourcePosition_;
          MT_Vector2D                    vTargetPosition_;
          uint                           nTargetKnowledgeID_;
          uint                           nNbrAmmoFired_;
          bool                           bIsFlying_;
          MT_Float                       rImpactTimeStep_;
          PHY_IndirectFireResults*       pFireResult_;
};

#include "MIL_Effect_IndirectFire.inl"

#endif // __MIL_Effect_IndirectFire_h_
