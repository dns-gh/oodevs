// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalHumanState.h $
// $Author: Jvt $
// $Modtime: 29/04/05 11:15 $
// $Revision: 6 $
// $Workfile: PHY_MedicalHumanState.h $
//
// *****************************************************************************

#ifndef __PHY_MedicalHumanState_h_
#define __PHY_MedicalHumanState_h_

#include "MIL.h"

class Human_ABC;
class PHY_MedicalConsign_ABC;
class MIL_AgentPion;
class MIL_Automate;
class PHY_ComposantePion;

// =============================================================================
// @class  PHY_MedicalHumanState
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MedicalHumanState : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     PHY_MedicalHumanState( MIL_AgentPion& pion, Human_ABC& humann, bool bEvacuatedByThirdParty = false );
     PHY_MedicalHumanState();
    virtual ~PHY_MedicalHumanState();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Accessors
    //@{
    const MIL_Automate& GetAutomate          () const;
    const Human_ABC&    GetHuman             () const;
    const MT_Vector2D&  GetHumanPosition     () const;
          void          SetHumanPosition     ( const MT_Vector2D& vPosition );
          bool          NeedDiagnosis        () const;
          bool          NeedSorting          () const;
          bool          IsAnEmergency        () const;
          bool          IsInAmbulance        () const;
          bool          EvacuatedByThirdParty() const;
          void          NotifyDiagnosed      ();
          void          NotifySorted         ();
    //@}

    //! @name Operations
    //@{
    void NotifyHumanChanged    ();
    void NotifyHandledByMedical();
    bool GoBackToWar           ();
    void Cancel                ();
    unsigned int Heal          ( const PHY_ComposantePion& doctor );
    //@}

    //! @name Consign
    //@{
    void SetConsign( PHY_MedicalConsign_ABC* pConsign );
    //@}

    //! @name Network
    //@{
    void SendChangedState() const;
    void SendFullState   ( unsigned int context ) const;
    void Clean           ();
    //@}

private:
    //! @name Network tools
    //@{
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned int nID_;
    const unsigned int nCreationTick_;
    MIL_AgentPion* pPion_;
    Human_ABC* pHuman_;
    PHY_MedicalConsign_ABC* pConsign_;
    MT_Vector2D vHumanPosition_; // Position de l'humain lors de son traitement dans la chaine medical
    bool bShouldGoBackToWar_;
    bool bHasChanged_;
    bool bHumanStateHasChanged_;
    bool bDiagnosed_;
    bool bSorted_;
    bool bHandledByMedical_;
    bool bEvacuatedByThirdParty_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_MedicalHumanState )

#endif // __PHY_MedicalHumanState_h_
