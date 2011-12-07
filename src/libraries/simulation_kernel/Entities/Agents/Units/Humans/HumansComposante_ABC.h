// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HumansComposante_ABC_h_
#define __HumansComposante_ABC_h_

class Human_ABC;
class PHY_HumanRank;
class PHY_HumanWound;
class PHY_ComposantePion;
class PHY_ComposanteState;
class PHY_FireDamages_Agent;
class PHY_RoleInterface_Composantes;
class MIL_ToxicEffectManipulator;
class MIL_BurnEffectManipulator;
class MIL_FloodEffectManipulator;
class MIL_AutomateLOG;
class MIL_Injury_ABC;
class PHY_InjuredHuman;

namespace sword
{
    class MissionParameters;
}

// =============================================================================
// @class  HumansComposante_ABC
// Created: MGD 2010-03-18
// =============================================================================
class HumansComposante_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             HumansComposante_ABC() {}
    virtual ~HumansComposante_ABC() {}
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}

    //! @name Operations
    //@{
    virtual bool ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound ) = 0;

    virtual void ApplyContamination( const MIL_ToxicEffectManipulator& contamination ) = 0;
    virtual void ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination ) = 0;
    virtual void ApplyInjury( MIL_Injury_ABC& injury ) = 0;
    virtual void ApplyWounds( const PHY_ComposanteState& newCompState, PHY_FireDamages_Agent& fireDamages ) = 0;
    virtual void ApplyBurn( const MIL_BurnEffectManipulator& burn ) = 0;
    virtual void ApplyFlood( const MIL_FloodEffectManipulator& flood ) = 0;
    virtual void HealAllHumans() = 0;
    virtual void KillAllHumans() = 0;
    virtual unsigned int HealHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange ) = 0;
    virtual unsigned int OverloadHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound, bool psyop = false, bool contaminated = false ) = 0;
    virtual unsigned int WoundHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound ) = 0;
    virtual void ChangeHumanState( sword::MissionParameters& msg ) = 0;

    virtual PHY_InjuredHuman* GetInjury() = 0;
    virtual double GetOperationalState() const = 0;
    virtual bool IsViable() const = 0;
    virtual bool IsEmpty() const = 0;
    //@}

    //! @name Composante notifications
    //@{
    virtual void NotifyComposanteHandledByMaintenance() = 0;
    virtual void NotifyComposanteBackFromMaintenance () = 0;
    virtual void NotifyComposanteTransfered          ( PHY_RoleInterface_Composantes& src, PHY_RoleInterface_Composantes& dest ) = 0;
    //@}

    //! @name Humans notifications
    //@{
    virtual void NotifyHumanAdded  ( Human_ABC& human ) = 0;
    virtual void NotifyHumanRemoved( Human_ABC& human ) = 0;
    virtual void NotifyHumanChanged( Human_ABC& human, const Human_ABC& copyOfOldHumanState ) = 0;
    //@}

    //! @name Medical
    //@{
    virtual bool HasWoundedHumansToEvacuate() const = 0;
    virtual void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_ComposantePion& GetComposante() const = 0;
    virtual unsigned int GetNbrUsableHumans() const = 0;
    //@}
};

#endif // __HumansComposante_ABC_h_
