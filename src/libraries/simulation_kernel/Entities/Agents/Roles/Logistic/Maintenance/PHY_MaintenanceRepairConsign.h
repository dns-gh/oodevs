// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceRepairConsign.h $
// $Author: Jvt $
// $Modtime: 11/04/05 16:53 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceRepairConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceRepairConsign_h_
#define __PHY_MaintenanceRepairConsign_h_

#include "PHY_MaintenanceConsign_ABC.h"

class PHY_RolePionLOG_Maintenance;

// =============================================================================
// @class  PHY_MaintenanceRepairConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceRepairConsign : public PHY_MaintenanceConsign_ABC
{

public:
             PHY_MaintenanceRepairConsign( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState );
             PHY_MaintenanceRepairConsign();
    virtual ~PHY_MaintenanceRepairConsign();

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
    bool DoWaitingForParts                 ();
    bool DoWaitingForRepairer              (); 
    void DoReturnComposante                ();
    bool DoSearchForCarrier                ();

    void EnterStateWaitingForCarrier       ();
    void EnterStateWaitingForParts         ();
    void EnterStateWaitingForRepairer      ();
    void EnterStateRepairing               ();
    void EnterStateGoingBackToWar          ();
    //@}

private:
    PHY_ComposantePion* pRepairer_;
};

#endif // __PHY_MaintenanceRepairConsign_h_
