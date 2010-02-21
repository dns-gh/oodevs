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

#include "simulation_kernel/Entities/Agents/Units/Humans/PHY_Human.h"

class PHY_HumanRank;
class PHY_HumanWound;
class PHY_ComposantePion;
class PHY_ComposanteState;
class PHY_FireDamages_Agent;
class PHY_RoleInterface_Composantes;
class PHY_MedicalHumanState;
class MIL_ToxicEffectManipulator;
class MIL_AutomateLOG;
class MIL_Time_ABC;
class MIL_Injury_ABC;

// =============================================================================
// @class  PHY_HumansComposante
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumansComposante : private boost::noncopyable
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
    bool ChangeHumanRank   ( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound );

    void ApplyContamination( const MIL_ToxicEffectManipulator& contamination );
    void ApplyPoisonous    ( const MIL_ToxicEffectManipulator& contamination );
    void ApplyInjury       ( MIL_Injury_ABC& injury );
    void ApplyWounds       ( const PHY_ComposanteState& newCompState, PHY_FireDamages_Agent& fireDamages );
    unsigned int WoundHumans       ( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound );
    unsigned int HealHumans        ( const PHY_HumanRank& rank, unsigned int nNbrToChange );
    void HealAllHumans     ();    

    MT_Float GetOperationalState() const;
    bool     IsViable           () const;
    //@}

    //! @name Composante notifications
    //@{
    void NotifyComposanteHandledByMaintenance();
    void NotifyComposanteBackFromMaintenance ();
    void NotifyComposanteTransfered          ( PHY_RoleInterface_Composantes& src, PHY_RoleInterface_Composantes& dest );
    //@}

    //! @name Humans notifications
    //@{
    void NotifyHumanAdded  ( PHY_Human& human );
    void NotifyHumanRemoved( PHY_Human& human );
    void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState );
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
    PHY_Human::T_HumanVector humans_;
    unsigned int nNbrUsableHumans_;
};

BOOST_CLASS_EXPORT_KEY( PHY_HumansComposante )

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_HumansComposante::serialize( Archive& file, const unsigned int )
{
    file & pComposante_
         & humans_
         & nNbrUsableHumans_;
    assert( pComposante_ );
}

#endif // __PHY_HumansComposante_h_
