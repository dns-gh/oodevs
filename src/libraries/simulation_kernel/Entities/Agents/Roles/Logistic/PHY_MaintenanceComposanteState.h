// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.h $
// $Author: Jvt $
// $Modtime: 4/05/05 10:42 $
// $Revision: 4 $
// $Workfile: PHY_MaintenanceComposanteState.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceComposanteState_h_
#define __PHY_MaintenanceComposanteState_h_

#include "MIL.h"
#include <boost/optional.hpp>

class PHY_ComposantePion;
class PHY_ComposanteTypePion;
class PHY_MaintenanceConsign_ABC;
class PHY_Breakdown;
class MIL_Agent_ABC;
class MIL_Automate;

// =============================================================================
// @class  PHY_MaintenanceComposanteState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceComposanteState : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
              PHY_MaintenanceComposanteState( MIL_Agent_ABC& pion, PHY_ComposantePion& composante );
              PHY_MaintenanceComposanteState();
     virtual ~PHY_MaintenanceComposanteState();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    const MIL_Automate& GetAutomate() const;
    const PHY_ComposantePion& GetComposante() const;
    const PHY_Breakdown& GetComposanteBreakdown() const;
    const MT_Vector2D& GetComposantePosition() const;
    void SetComposantePosition( const MT_Vector2D& vPosition );
    const MT_Vector2D& GetPionPosition() const;
    unsigned int ApproximateTravelTime ( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const;
    bool NeedDiagnosis() const;
    void NotifyDiagnosed();
    //@}

    //! @name Operations
    //@{
    void NotifyHandledByMaintenance();
    void NotifyRepaired();
    void Cancel();
    //@}

    //! @name Consign
    //@{
    void SetConsign( PHY_MaintenanceConsign_ABC* pConsign );
    PHY_MaintenanceConsign_ABC* GetConsign() const;

    void SelectNewState();
    void TransferToLogisticSuperior();
    void SelectMaintenanceTransporter( const PHY_ComposanteTypePion& type, boost::optional< const MIL_Agent_ABC& > destination );
    void SelectDiagnosisTeam( const PHY_ComposanteTypePion& type );
    void SelectRepairTeam( const PHY_ComposanteTypePion& type );
    //@}

    //! @name Network
    //@{
    void SendChangedState() const;
    void SendFullState( unsigned int context ) const;
    void Clean();
    //@}

private:
    //! @name Network tools
    //@{
    void SendMsgCreation() const;
    void SendMsgDestruction() const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int nID_;
    const unsigned int nCreationTick_;
    MIL_Agent_ABC* pPion_;
    PHY_ComposantePion* pComposante_;
    PHY_MaintenanceConsign_ABC* pConsign_;
    MT_Vector2D vComposantePosition_; // Position de la composante lors de son traitement dans la chaine maintenance
    bool bHasChanged_;
    bool bDiagnosed_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_MaintenanceComposanteState )

#endif // __PHY_MaintenanceComposanteState_h_
