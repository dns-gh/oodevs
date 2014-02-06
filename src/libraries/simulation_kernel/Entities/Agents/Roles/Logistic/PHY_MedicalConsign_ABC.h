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

namespace sword
{
    enum LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus;
}

class MIL_Agent_ABC;
class PHY_RoleInterface_Medical;
class PHY_MedicalHumanState;

// =============================================================================
// @class  PHY_MedicalConsign_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalConsign_ABC : private boost::noncopyable
{
public:
             PHY_MedicalConsign_ABC( MIL_Agent_ABC& medical, PHY_MedicalHumanState& humanState );
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

            void FinishSuccessfullyWithoutDelay();

    bool HasValidHumanState() const;
    void ClearConsign();
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
    //! @name
    void EnterStateFinished();

    sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus GetState() const;
    void SetState( sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus nNewState, int timer );
    bool DecrementTimer();
    void SendExternalTimerValue( int timer );

    PHY_RoleInterface_Medical& GetPionMedical() const;
    //@}

private:
    void ResetTimer( int timer );

private:
    int nTimer_;
    sword::LogMedicalHandlingUpdate_EnumLogMedicalHandlingStatus nState_;
    int32_t currentStateEndTimeStep_; // Only used to send the information over the network
    bool bHasChanged_;
    MIL_Agent_ABC* pMedical_;

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
    file & pMedical_;
    file & pHumanState_;
    file & nTimer_;
    file & nState_;
    file & currentStateEndTimeStep_;
}

#endif // __PHY_MedicalConsign_ABC_h_
