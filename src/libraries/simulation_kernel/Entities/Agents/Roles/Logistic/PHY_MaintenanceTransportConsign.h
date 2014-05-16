// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MaintenanceTransportConsign.h $
// $Author: Jvt $
// $Modtime: 11/04/05 16:56 $
// $Revision: 2 $
// $Workfile: PHY_MaintenanceTransportConsign.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceTransportConsign_h_
#define __PHY_MaintenanceTransportConsign_h_

#include "PHY_MaintenanceConsign_ABC.h"

class PHY_ComposantePion;

// =============================================================================
// @class  PHY_MaintenanceTransportConsign
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceTransportConsign : public PHY_MaintenanceConsign_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_MaintenanceTransportConsign( MIL_Agent_ABC& maintenanceAgent, const boost::shared_ptr< PHY_MaintenanceComposanteState >& state );
             PHY_MaintenanceTransportConsign();
    virtual ~PHY_MaintenanceTransportConsign();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Cancel();
    virtual bool Update();

    virtual bool SearchForUpperLevelNotFound() const;

    virtual void SelectNewState();
    virtual void TransferToLogisticSuperior();
    virtual void SelectMaintenanceTransporter( const PHY_ComposanteTypePion& type, const MIL_Agent_ABC* destination );
    virtual void SelectDiagnosisTeam( const PHY_ComposanteTypePion& type );
    virtual void SelectRepairTeam( const PHY_ComposanteTypePion& type );
    //@}

private:
    //! @name States
    //@{
    bool DoWaitingForCarrier               ();
    void DoWaitingForCarrierSelection      ();
    bool DoSearchForUpperLevel             ();

    void EnterStateWaitingForCarrier       ();
    void EnterStateGoingFrom               ();
    void EnterStateCarrierGoingTo          ();
    void EnterStateCarrierLoading          ();
    void EnterStateCarrierGoingFrom        ();
    void EnterStateCarrierUnloading        ();

    void ChooseStateAfterTransport         ();
    //@}

    //! @name Helpers
    //@{
    void ResetComponent();
    bool FindAlternativeTransportUnit( const PHY_ComposanteTypePion* type = 0 );
    //@}

private:
    //! @name Member data
    //@{
    PHY_ComposantePion* component_;
    const MIL_Agent_ABC* destination_;
    bool searchForUpperLevelDone_;
    std::function< void() > next_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_MaintenanceTransportConsign )

#endif // __PHY_MaintenanceTransportConsign_h_
