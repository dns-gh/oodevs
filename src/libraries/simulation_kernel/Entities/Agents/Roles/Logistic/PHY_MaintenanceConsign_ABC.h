// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/roles/logistic/maintenance/PHY_MaintenanceConsign_ABC.h $
// $Author: Jvt $
// $Modtime: 4/05/05 10:43 $
// $Revision: 4 $
// $Workfile: PHY_MaintenanceConsign_ABC.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceConsign_ABC_h_
#define __PHY_MaintenanceConsign_ABC_h_

namespace client
{
    class LogMaintenanceHandlingUpdate;
}

class PHY_MaintenanceComposanteState;
class MIL_Agent_ABC;
class PHY_RoleInterface_Maintenance;
class PHY_ComposanteTypePion;
class PHY_Breakdown;

// =============================================================================
// @class  PHY_MaintenanceConsign_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceConsign_ABC : private boost::noncopyable
{
public:
             PHY_MaintenanceConsign_ABC( MIL_Agent_ABC& maintenanceAgent, PHY_MaintenanceComposanteState& composanteState );
             PHY_MaintenanceConsign_ABC();
    virtual ~PHY_MaintenanceConsign_ABC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Accessors
    //@{
          bool                            IsFinished            () const;
    const PHY_ComposanteTypePion&         GetComposanteType     () const;
    const PHY_Breakdown&                  GetComposanteBreakdown() const;
    const PHY_MaintenanceComposanteState& GetComposanteState    () const;
    //@}

    //! @name Operations
    //@{
    virtual void Cancel();
    virtual bool Update() = 0;

            void Clean     ();
            bool HasChanged() const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( client::LogMaintenanceHandlingUpdate& asn ) const;
    void SendFullState   ( client::LogMaintenanceHandlingUpdate& asn ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {
        eGoingFrom,         // La composante se déplace seule vers le TC2
        eWaitingForCarrier,
        eCarrierGoingTo,
        eCarrierLoading,
        eCarrierGoingFrom,
        eCarrierUnloading,
        eDiagnosing,
        eSearchingForUpperLevel,
        eWaitingForParts,
        eWaitingForRepairer,
        eRepairing,
        eGoingBackToWar,
        eFinished
    };
    //@}

protected:
    //! @name Tools
    //@{
    void EnterStateFinished();
    E_State GetState() const;
    void    SetState      ( E_State nNewState );
    void    ResetTimer    ( int timer );
    bool    DecrementTimer();

    PHY_RoleInterface_Maintenance& GetPionMaintenance() const;
    //@}

    MIL_Agent_ABC*                  pMaintenance_;

private:
    E_State  nState_;
    int      nTimer_;
    unsigned currentStateEndTimeStep_; // Only used to send the information over the network
    bool     bHasChanged_;

protected:
    PHY_MaintenanceComposanteState* pComposanteState_;
};


// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceConsign_ABC::serialize( Archive& file, const unsigned int )
{
    file & pMaintenance_
         & pComposanteState_
         & nState_
         & nTimer_
         & currentStateEndTimeStep_;
}

#endif // __PHY_MaintenanceConsign_ABC_h_
