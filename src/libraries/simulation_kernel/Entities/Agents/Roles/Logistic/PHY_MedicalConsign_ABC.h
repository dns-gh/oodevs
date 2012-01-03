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

namespace client
{
    class LogMedicalHandlingUpdate;
}

class PHY_RoleInterface_Medical;
class PHY_MedicalHumanState;

// =============================================================================
// @class  PHY_MedicalConsign_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalConsign_ABC : private boost::noncopyable
{
public:
             PHY_MedicalConsign_ABC( PHY_RoleInterface_Medical& medical, PHY_MedicalHumanState& humanState );
             PHY_MedicalConsign_ABC();
    virtual ~PHY_MedicalConsign_ABC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
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
    void SendChangedState( client::LogMedicalHandlingUpdate& asn ) const;
    void SendFullState   ( client::LogMedicalHandlingUpdate& asn ) const;
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
        eResting,
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
    void ResetTimer( int timer );
    bool DecrementTimer();
    void SendExternalTimerValue( int timer );

    PHY_RoleInterface_Medical& GetPionMedical() const;
    //@}

private:
    int                         nTimer_;
    E_State                     nState_;
    unsigned                    currentStateEndTimeStep_; // Only used to send the information over the network
    bool                        bHasChanged_;
    PHY_RoleInterface_Medical*  pMedical_;

protected:
    PHY_MedicalHumanState*   pHumanState_;
   
};

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MedicalConsign_ABC::serialize( Archive& file, const unsigned int )
{
    file & pMedical_
         & pHumanState_
         & nTimer_
         & nState_
         & currentStateEndTimeStep_;
}

#endif // __PHY_MedicalConsign_ABC_h_
