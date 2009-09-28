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

class PHY_MaintenanceComposanteState;
class MIL_Agent_ABC;
class PHY_RoleInterface_Maintenance;
class PHY_ComposanteTypePion;
class PHY_ComposantePion;
class PHY_Breakdown;
class MIL_AgentPionLOG_ABC;
class NET_ASN_MsgLogMaintenanceHandlingUpdate;

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
    template< typename Archive > void serialize( Archive&, const uint );
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
    void SendChangedState( NET_ASN_MsgLogMaintenanceHandlingUpdate& asn ) const;
    void SendFullState   ( NET_ASN_MsgLogMaintenanceHandlingUpdate& asn ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_State
    {
        eGoingFrom,         // La composante se d�place seule vers le TC2
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
    //! @name
    //@{
    void EnterStateFinished();

    E_State GetState() const;
    //@}

protected:
    //! @name Tools
    //@{
    void SetState( E_State nNewState );    

    PHY_RoleInterface_Maintenance& GetPionMaintenance() const;
    //@}

    MIL_Agent_ABC*                  pMaintenance_;
private:
    E_State                         nState_;
    bool                            bHasChanged_;

protected:
    int                             nTimer_;        
    PHY_MaintenanceComposanteState* pComposanteState_;
};


// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::serialize
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_MaintenanceConsign_ABC::serialize( Archive& file, const uint )
{
    file & pMaintenance_
         & pComposanteState_
         & nState_
         & nTimer_;
}

#endif // __PHY_MaintenanceConsign_ABC_h_
