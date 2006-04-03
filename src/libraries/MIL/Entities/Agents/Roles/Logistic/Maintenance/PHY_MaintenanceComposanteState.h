// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceComposanteState.h $
// $Author: Jvt $
// $Modtime: 4/05/05 10:42 $
// $Revision: 4 $
// $Workfile: PHY_MaintenanceComposanteState.h $
//
// *****************************************************************************

#ifndef __PHY_MaintenanceComposanteState_h_
#define __PHY_MaintenanceComposanteState_h_

#include "MIL.h"

class PHY_ComposantePion;
class PHY_MaintenanceConsign_ABC;
class PHY_Breakdown;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_MaintenanceComposanteState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MaintenanceComposanteState
{
    MT_COPYNOTALLOWED( PHY_MaintenanceComposanteState )

public:
     PHY_MaintenanceComposanteState( MIL_AgentPion& pion, PHY_ComposantePion& composante );
     PHY_MaintenanceComposanteState();
    ~PHY_MaintenanceComposanteState();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Accessors
    //@{
    const MIL_Automate&       GetAutomate           () const;
    const PHY_ComposantePion& GetComposante         () const;
    const PHY_Breakdown&      GetComposanteBreakdown() const;
    const MT_Vector2D&        GetComposantePosition () const;
          void                SetComposantePosition ( const MT_Vector2D& vPosition );
    const MT_Vector2D&        GetPionPosition       () const;          
          uint                ApproximateTravelTime ( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const;
          bool                NeedDiagnosis         () const;
          void                NotifyDiagnosed       ();
    //@}

    //! @name Operations
    //@{
    void NotifyHandledByMaintenance();
    void NotifyRepaired            ();
    void Cancel                    ();
    //@}

    //! @name Consign
    //@{
    void SetConsign( PHY_MaintenanceConsign_ABC* pConsign );
    //@}

    //! @name Network
    //@{
    void SendChangedState() const;
    void SendFullState   () const;
    void Clean           ();
    //@}

private:
    //! @name Network tools
    //@{
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

private:
    const uint                        nID_;
          MIL_AgentPion*              pPion_;
          PHY_ComposantePion*         pComposante_;
          PHY_MaintenanceConsign_ABC* pConsign_;
          MT_Vector2D                 vComposantePosition_; // Position de la composante lors de son traitement dans la chaine maintenance
          bool                        bHasChanged_;
          bool                        bDiagnosed_;
};

#include "PHY_MaintenanceComposanteState.inl"

#endif // __PHY_MaintenanceComposanteState_h_
