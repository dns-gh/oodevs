// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumansComposante.h $
// $Author: Jvt $
// $Modtime: 1/04/05 11:25 $
// $Revision: 6 $
// $Workfile: PHY_HumansComposante.h $
//
// *****************************************************************************

#ifndef __PHY_HumansComposante_h_
#define __PHY_HumansComposante_h_

#include "HumansComposante_ABC.h"
#include <boost/serialization/export.hpp>

class Human_ABC;
class PHY_HumanRank;
class PHY_HumanWound;
class PHY_ComposantePion;
class PHY_ComposanteState;
class PHY_FireDamages_Agent;
class PHY_RoleInterface_Composantes;
class PHY_InjuredHuman;
class MIL_AutomateLOG;
class MIL_Time_ABC;
class MIL_Injury_ABC;

namespace sword
{
    class MissionParameters;
}

// =============================================================================
// @class  PHY_HumansComposante
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumansComposante : public HumansComposante_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_HumansComposante();
             PHY_HumansComposante( const MIL_Time_ABC& time, PHY_ComposantePion& composante, unsigned int nNbrMdr );
    virtual ~PHY_HumansComposante();
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    bool ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound );

    void ApplyContamination( const MIL_ToxicEffectManipulator& contamination );
    void ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination );
    void ApplyBurn( const MIL_BurnEffectManipulator& burn );
    void ApplyFlood( const MIL_FloodEffectManipulator& flood );
    void ApplyInjury( MIL_Injury_ABC& injury );
    PHY_InjuredHuman* GetInjury();
    void ApplyWounds( const PHY_ComposanteState& newCompState, PHY_FireDamages_Agent& fireDamages );
    void HealAllHumans();
    unsigned int HealHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange );
    unsigned int OverloadHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound, bool psyop = false, bool contaminated = false );
    unsigned int WoundHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound );
    void ChangeHumanState( sword::MissionParameters& msg );
    void ChangeHumanSize( unsigned int newHumanSize );

    double GetOperationalState() const;
    bool IsViable() const;
    bool IsEmpty() const;
    //@}

    //! @name Composante notifications
    //@{
    void NotifyComposanteHandledByMaintenance();
    void NotifyComposanteBackFromMaintenance();
    void NotifyComposanteTransfered( PHY_RoleInterface_Composantes& src, PHY_RoleInterface_Composantes& dest );
    //@}

    //! @name Humans notifications
    //@{
    void NotifyHumanAdded( Human_ABC& human );
    void NotifyHumanRemoved( Human_ABC& human );
    void NotifyHumanChanged( Human_ABC& human, const Human_ABC& copyOfOldHumanState );
    //@}

    //! @name Medical
    //@{
    bool HasWoundedHumansToEvacuate() const;
    void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_ComposantePion& GetComposante() const;
    unsigned int GetNbrUsableHumans() const;
    //@}

private:
    PHY_ComposantePion* pComposante_;
    std::vector< Human_ABC* > humans_;
    unsigned int nNbrUsableHumans_;
    PHY_InjuredHuman* injury_;
};

BOOST_CLASS_EXPORT_KEY( PHY_HumansComposante )

#endif // __PHY_HumansComposante_h_
