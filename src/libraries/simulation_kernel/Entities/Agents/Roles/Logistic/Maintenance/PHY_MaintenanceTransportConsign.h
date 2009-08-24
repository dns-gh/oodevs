// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceTransportConsign.h $
// $Author: Jvt $
// $Modtime: 11/04/05 16:56 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceTransportConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceTransportConsign_h_
#define __PHY_MaintenanceTransportConsign_h_

#include "PHY_MaintenanceConsign_ABC.h"

class PHY_RoleInterface_Maintenance;

// =============================================================================
// @class  PHY_MaintenanceTransportConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceTransportConsign : public PHY_MaintenanceConsign_ABC
{

public:
             PHY_MaintenanceTransportConsign( PHY_RoleInterface_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState );
             PHY_MaintenanceTransportConsign();
    virtual ~PHY_MaintenanceTransportConsign();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual void Cancel();
    virtual bool Update(); 
    //@}

private:
    //! @name 
    //@{
    bool DoWaitingForCarrier               ();
    bool DoSearchForUpperLevel             ();  

    void EnterStateWaitingForCarrier       ();
    void EnterStateGoingFrom               ();
    void EnterStateCarrierGoingTo          ();
    void EnterStateCarrierLoading          ();
    void EnterStateCarrierGoingFrom        ();
    void EnterStateCarrierUnloading        ();
    void EnterStateDiagnosing              ();
    void ChooseStateAfterDiagnostic        ();
    //@}

private:
    PHY_ComposantePion* pCarrier_;
};

#endif // __PHY_MaintenanceTransportConsign_h_
