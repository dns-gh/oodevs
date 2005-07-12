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

#include "MIL.h"

class PHY_MaintenanceComposanteState;
class PHY_RolePionLOG_Maintenance;
class PHY_ComposanteTypePion;
class PHY_ComposantePion;
class PHY_Breakdown;
class MIL_AgentPionLOG_ABC;
class NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate;

// =============================================================================
// @class  PHY_MaintenanceConsign_ABC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceConsign_ABC
{
    MT_COPYNOTALLOWED( PHY_MaintenanceConsign_ABC )

public:
             PHY_MaintenanceConsign_ABC( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState );
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
            void Cancel();
    virtual bool Update() = 0;

            void Clean     ();
            bool HasChanged() const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate& asn ) const;
    void SendFullState   ( NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate& asn ) const;
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
        eWaitingForGoingBackToWar,
        eGoingBackToWar,
        eFinished
    };
    //@}

protected:
    //! @name
    //@{
    bool DoWaitingForCarrier               ();
    bool DoSearchingForUpperLevel          ();  
    bool DoWaitingForParts                 ();
    bool DoWaitingForRepairer              (); 
    bool DoWaitingForGoingBackToWar        ();
    void DoReturnComposante                ();

    void EnterStateWaitingForCarrier       ();
    void EnterStateGoingFrom               ();
    void EnterStateCarrierGoingTo          ();
    void EnterStateCarrierLoading          ();
    void EnterStateCarrierGoingFrom        ();
    void EnterStateCarrierUnloading        ();
    void EnterStateDiagnosing              ();
    void ChooseStateAfterDiagnostic        ();

    void EnterStateWaitingForParts         ();
    void EnterStateWaitingForRepairer      ();
    void EnterStateRepairing               ();
    void EnterStateWaitingForGoingBackToWar();
    void EnterStateGoingBackToWar          ();
    void EnterStateFinished                ();

    E_State GetState() const;
    //@}

private:
    //! @name Tools
    //@{
    void SetState( E_State nNewState );    
    //@}

private:
    PHY_RolePionLOG_Maintenance*    pMaintenance_;
    PHY_MaintenanceComposanteState* pComposanteState_;
    PHY_ComposantePion*             pActor_; // Carrier / repairer ...
    E_State                         nState_;
    bool                            bHasChanged_;

protected:
    int                             nTimer_;        
};

#include "PHY_MaintenanceConsign_ABC.inl"

#endif // __PHY_MaintenanceConsign_ABC_h_
