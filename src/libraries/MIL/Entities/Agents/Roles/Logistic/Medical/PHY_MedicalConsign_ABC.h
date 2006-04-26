// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/roles/logistic/medical/PHY_MedicalConsign_ABC.h $
// $Author: Jvt $
// $Modtime: 3/05/05 16:51 $
// $Revision: 7 $
// $Workfile: PHY_MedicalConsign_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalConsign_ABC_h_
#define __PHY_MedicalConsign_ABC_h_

#include "MIL.h"

class PHY_RolePion_Medical;
class PHY_RolePionLOG_Medical;
class PHY_ComposantePion;
class PHY_MedicalHumanState;
class PHY_MedicalAmbulance;
class NET_ASN_MsgLogSanteTraitementHumainUpdate;
class MIL_Automate;

// =============================================================================
// @class  PHY_MedicalConsign_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalConsign_ABC
{
    MT_COPYNOTALLOWED( PHY_MedicalConsign_ABC )

public:
             PHY_MedicalConsign_ABC( PHY_RolePionLOG_Medical& medical, PHY_MedicalHumanState& humanState );
             PHY_MedicalConsign_ABC();
    virtual ~PHY_MedicalConsign_ABC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual void Cancel();
    virtual bool Update() = 0;

            void Clean     ();
            bool HasChanged() const;
    //@}

    //! @name Accessors
    //@{
    virtual      bool                   CouldNeedCollectionAmbulance() const = 0;
    virtual      bool                   IsATransportConsign         () const = 0;
                 bool                   IsAnEmergency               () const;
                 bool                   IsFinished                  () const;
           const PHY_MedicalHumanState& GetHumanState               () const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgLogSanteTraitementHumainUpdate& asn ) const;
    void SendFullState   ( NET_ASN_MsgLogSanteTraitementHumainUpdate& asn ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {
        eWaitingForEvacuation,
        eEvacuationGoingTo,
        eEvacuationLoading,
        eEvacuationWaitingForFullLoading,
        eEvacuationGoingFrom,
        eEvacuationUnloading,
        eWaitingForDiagnostic,
        eDiagnosing,
        eSearchingForSortingArea,
        eWaitingForSorting,
        eSorting,
        eSearchingForHealingArea,
        eWaitingForHealing,
        eHealing,
        eWaitingForCollection,
        eCollectionLoading,
        eCollectionWaitingForFullLoading,
        eCollectionGoingTo,
        eCollectionUnloading,
//        eCollectionGoingFrom,           Juste pour les ambulances
        eFinished
   };
    //@}

protected:
    //! @name
    void EnterStateFinished();   

    E_State GetState() const;
    void    SetState( E_State nNewState );

    PHY_RolePionLOG_Medical& GetPionMedical();
    //@}

private:
    E_State                  nState_;
    bool                     bHasChanged_;
    PHY_RolePionLOG_Medical* pMedical_;

protected:
    PHY_MedicalHumanState*   pHumanState_;
    int                      nTimer_;        
};

#include "PHY_MedicalConsign_ABC.inl"

#endif // __PHY_MedicalConsign_ABC_h_
