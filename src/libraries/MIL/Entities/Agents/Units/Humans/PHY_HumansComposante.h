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
class PHY_AgentFireResult;
class MIL_NbcAgentType;
class MIL_AutomateLOG;
class PHY_Human;
class PHY_RolePion_Composantes;

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
    
    //! @name 
    //@{
    bool ChangeHumanRank  ( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound );

    void ApplyWounds      ( const MIL_NbcAgentType& nbcAgentType );
    void ApplyWounds      ( const PHY_ComposanteState& newCompState, PHY_AgentFireResult& fireResult );
    void KillAllHumans    ();
    void KillAllHumans    ( PHY_AgentFireResult& fireResult );

    uint WoundHumans      ( uint nNbrToChange, const PHY_HumanWound& newWound, const PHY_HumanRank* pHumanRank = 0 );
    void HealAllHumans    ();
    uint HealHumans       ( uint nNbrToChange );
    //@}

    //! @name Medical
    //@{
    void EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const;
    //@}

    //! @name Composante notifications
    //@{
    void NotifyComposanteHandledByMaintenance();
    void NotifyComposanteBackFromMaintenance ();
    void NotifyComposanteTransfered          ( PHY_RolePion_Composantes& src, PHY_RolePion_Composantes& dest );
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
};

#include "PHY_HumansComposante.inl"

#endif // __PHY_HumansComposante_h_
