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

#include "MIL.h"

class PHY_HumanRank;
class PHY_HumanWound;
class PHY_ComposantePion;
class PHY_ComposanteState;
class PHY_FireDamages_Agent;
class PHY_Human;
class PHY_RolePion_Composantes;
class PHY_MedicalHumanState;
class MIL_NbcAgent;
class MIL_AutomateLOG;

// =============================================================================
// @class  PHY_HumansComposante
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumansComposante
{
    MT_COPYNOTALLOWED( PHY_HumansComposante )

public:
     PHY_HumansComposante( PHY_ComposantePion& composante, uint nNbrMdr );
     PHY_HumansComposante();
    ~PHY_HumansComposante();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    bool ChangeHumanRank   ( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound );

    void ApplyContamination( const MIL_NbcAgent& nbcAgent );
    void ApplyPoisonous    ( const MIL_NbcAgent& nbcAgent );
    void ApplyWounds       ( const PHY_ComposanteState& newCompState, PHY_FireDamages_Agent& fireDamages );
    uint WoundHumans       ( const PHY_HumanRank& rank, uint nNbrToChange, const PHY_HumanWound& newWound );
    uint HealHumans        ( const PHY_HumanRank& rank, uint nNbrToChange );
    void HealAllHumans     ();    

    MT_Float GetOperationalState() const;
    bool     IsViable           () const;
    //@}

    //! @name Composante notifications
    //@{
    void NotifyComposanteHandledByMaintenance();
    void NotifyComposanteBackFromMaintenance ();
    void NotifyComposanteTransfered          ( PHY_RolePion_Composantes& src, PHY_RolePion_Composantes& dest );
    //@}

    //! @name Humans notifications
    //@{
    void NotifyHumanAdded  ( PHY_Human& human );
    void NotifyHumanRemoved( PHY_Human& human );
    void NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState );
    //@}

    //! @name Medical
    //@{
    bool                   HasWoundedHumansToEvacuate      () const;
    void                   EvacuateWoundedHumans           ( MIL_AutomateLOG& destinationTC2 ) const;

    void                   NotifyHumanBackFromMedical      ( PHY_MedicalHumanState& humanState ) const;
    PHY_MedicalHumanState* NotifyHumanWaitingForMedical    ( PHY_Human& human ) const;
    PHY_MedicalHumanState* NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_ComposantePion& GetComposante     () const;
          uint                GetNbrUsableHumans() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_Human* >      T_HumanVector;
    typedef T_HumanVector::const_iterator  CIT_HumanVector;
    //@}

private:
    PHY_ComposantePion* pComposante_;
    T_HumanVector       humans_;
    uint                nNbrUsableHumans_;
};

#include "PHY_HumansComposante.inl"

#endif // __PHY_HumansComposante_h_
