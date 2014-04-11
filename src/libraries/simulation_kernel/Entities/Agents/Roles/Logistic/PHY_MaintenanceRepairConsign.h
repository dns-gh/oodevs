// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MaintenanceRepairConsign.h $
// $Author: Jvt $
// $Modtime: 11/04/05 16:53 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceRepairConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceRepairConsign_h_
#define __PHY_MaintenanceRepairConsign_h_

#include "PHY_MaintenanceConsign_ABC.h"

class PHY_ComposantePion;

// =============================================================================
// @class  PHY_MaintenanceRepairConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceRepairConsign : public PHY_MaintenanceConsign_ABC
{
public:
             PHY_MaintenanceRepairConsign( MIL_Agent_ABC& maintenanceAgent, const boost::shared_ptr< PHY_MaintenanceComposanteState >& state );
             PHY_MaintenanceRepairConsign();
    virtual ~PHY_MaintenanceRepairConsign();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Cancel();
    virtual bool Update();

    virtual void SelectNewState();
    virtual bool SearchForUpperLevelNotFound() const;
    virtual void TransferToLogisticSuperior();
    virtual void SelectMaintenanceTransporter( const PHY_ComposanteTypePion& type, boost::optional< const MIL_Agent_ABC& > destination );
    virtual void SelectDiagnosisTeam( const PHY_ComposanteTypePion& type );
    virtual void SelectRepairTeam( const PHY_ComposanteTypePion& type );
    //@}

private:
    //! @name Helpers
    //@{
    void ResetRepairer();
    bool FindAlternativeRepairTeam( const PHY_ComposanteTypePion* type = 0 );

    bool DoWaitingForParts();
    bool DoWaitingForRepairer();
    bool DoSearchForUpperLevel();
    void DoReturnComposante();
    bool DoSearchForCarrier();

    void EnterStateWaitingForCarrier();
    void EnterStateWaitingForParts();
    void EnterStateWaitingForRepairer();
    void EnterStateWaitingForRepairerSelection();
    void EnterStateRepairing();
    void EnterStateGoingBackToWar();
    //@}

private:
    PHY_ComposantePion* pRepairer_;
    bool searchForUpperLevelDone_;
    std::function< void() > next_;
};

BOOST_CLASS_EXPORT_KEY( PHY_MaintenanceRepairConsign )

#endif // __PHY_MaintenanceRepairConsign_h_
