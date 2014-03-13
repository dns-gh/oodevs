// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef PHY_MaintenanceDiagnosisConsign_h
#define PHY_MaintenanceDiagnosisConsign_h

#include "PHY_MaintenanceConsign_ABC.h"

class PHY_ComposantePion;

// =============================================================================
/** @class  PHY_MaintenanceDiagnosisConsign
    @brief  PHY_MaintenanceDiagnosisConsign
*/
// Created: SLI 2014-02-12
// =============================================================================
class PHY_MaintenanceDiagnosisConsign : public PHY_MaintenanceConsign_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_MaintenanceDiagnosisConsign( MIL_Agent_ABC& maintenanceAgent, PHY_MaintenanceComposanteState& composanteState );
             PHY_MaintenanceDiagnosisConsign();
    virtual ~PHY_MaintenanceDiagnosisConsign();
    //@}

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
    virtual void SelectMaintenanceTransporter( const PHY_ComposanteTypePion& type, const MIL_Agent_ABC* destination );
    virtual void SelectDiagnosisTeam( const PHY_ComposanteTypePion& type );
    virtual void SelectRepairTeam( const PHY_ComposanteTypePion& type );
    //@}

private:
    //! @name States
    //@{
    void EnterStateWaitingForDiagnosisTeam();
    void EnterStateDiagnosing             ();
    void ChooseStateAfterDiagnostic       ();
    bool DoSearchForUpperLevel            ();
    //@}

    //! @name Helpers
    //@{
    void ResetComponent();
    bool FindAlternativeDiagnosisTeam( const PHY_ComposanteTypePion* type = 0 );
    //@}

private:
    //! @name Member data
    //@{
    PHY_ComposantePion* component_;
    bool searchForUpperLevelDone_;
    std::function< void() > next_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_MaintenanceDiagnosisConsign )

#endif // PHY_MaintenanceDiagnosisConsign_h
