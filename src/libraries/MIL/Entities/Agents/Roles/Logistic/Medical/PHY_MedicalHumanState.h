// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalHumanState.h $
// $Author: Jvt $
// $Modtime: 29/04/05 11:15 $
// $Revision: 6 $
// $Workfile: PHY_MedicalHumanState.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalHumanState_h_
#define __PHY_MedicalHumanState_h_

#include "MIL.h"

#include "Tools/MIL_MOSIDManager.h"

class PHY_Human;
class PHY_MedicalConsign_ABC;
class MIL_AgentPion;
class MIL_Automate;
class PHY_ComposantePion;

// =============================================================================
// @class  PHY_MedicalHumanState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalHumanState
{
    MT_COPYNOTALLOWED( PHY_MedicalHumanState )

public:
     PHY_MedicalHumanState( MIL_AgentPion& pion, PHY_Human& humann, bool bEvacuatedByThirdParty = false );
     PHY_MedicalHumanState();
    ~PHY_MedicalHumanState();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Accessors
    //@{
    const MIL_Automate& GetAutomate          () const;
    const PHY_Human&    GetHuman             () const;
    const MT_Vector2D&  GetHumanPosition     () const;
          void          SetHumanPosition     ( const MT_Vector2D& vPosition );
    const MT_Vector2D&  GetPionPosition      () const;
          bool          NeedDiagnosis        () const;
          bool          IsAnEmergency        () const;
          bool          IsInAmbulance        () const;
          bool          EvacuatedByThirdParty() const;
          bool          ShouldGoBackToWar    () const;
          void          NotifyDiagnosed      ();     
    //@}

    //! @name Operations
    //@{
    void NotifyHumanChanged    (); // Called when the human state changed by PHY_Human
    void NotifyHandledByMedical();
    void GoBackToWar           ();
    void Cancel                ();
    void Heal                  ( const PHY_ComposantePion& doctor );
    //@}

    //! @name Consign
    //@{
    void SetConsign( PHY_MedicalConsign_ABC* pConsign );
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
    const uint                    nID_;
          MIL_AgentPion*          pPion_;
          PHY_Human*              pHuman_;
          PHY_MedicalConsign_ABC* pConsign_;
          MT_Vector2D             vHumanPosition_; // Position de l'humain lors de son traitement dans la chaine medical
          bool                    bShouldGoBackToWar_;
          bool                    bHasChanged_;
          bool                    bHumanStateHasChanged_;
          bool                    bDiagnosed_;
          bool                    bHandledByMedical_;
          bool                    bEvacuatedByThirdParty_;

public:
    static MIL_MOSIDManager idManager_;
};

#include "PHY_MedicalHumanState.inl"

#endif // __PHY_MedicalHumanState_h_
