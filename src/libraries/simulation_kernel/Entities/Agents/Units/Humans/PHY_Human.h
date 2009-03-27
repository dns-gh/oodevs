// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_Human.h $
// $Author: Jvt $
// $Modtime: 28/04/05 16:04 $
// $Revision: 6 $
// $Workfile: PHY_Human.h $
//
// *****************************************************************************

#ifndef __PHY_Human_h_
#define __PHY_Human_h_

#include "MIL.h"
#include "PHY_HumanWound.h"

class MIL_ToxicEffectManipulator;
class MIL_AutomateLOG;
class PHY_HumanRank;
class PHY_HumansComposante;
class PHY_MedicalHumanState;
class MIL_Time_ABC;

//$$$ Clarifier l'interface (trop de trucs différents pour trop de fonctionnalités proches) (Log vs Magic vs attrition)

// =============================================================================
// @class  PHY_Human
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Human
{
public:
    //! @name Types
    //@{
    enum E_Location
    {
        eBattleField,
        eMaintenance,
        eMedical
    };
    //@}

public:
     PHY_Human( const MIL_Time_ABC& time, PHY_HumansComposante& composante );
     PHY_Human( const PHY_Human& rhs );
     PHY_Human();
    ~PHY_Human();
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Heal                 (); // 'Magic' Heal
    bool SetRank              ( const PHY_HumanRank&  newRank  ); // Force rank change
    bool SetWound             ( const PHY_HumanWound& newWound ); // Don't test 'usability' or aggravation => force wound change
    bool ApplyWound           ( const PHY_HumanWound& newWound ); // Test 'usability'
    bool ApplyPoisonous       ( const MIL_ToxicEffectManipulator& nbcAgent ); // NBC effects
    void ApplyContamination   ( const MIL_ToxicEffectManipulator& nbcAgent ); // NBC effects
    void ApplyMentalDisease   ();
    void CancelLogisticRequest();
    //@}

    //! @name Accessors
    //@{
    const PHY_HumanRank&  GetRank         () const;
    const PHY_HumanWound& GetWound        () const;
          E_Location      GetLocation     () const;
          bool            IsUsable        () const;  
          bool            IsDead          () const;
          bool            IsWounded       () const;
          bool            IsContaminated  () const;
          bool            IsMentalDiseased() const;
          bool            IsAnEmergency   () const;
    //@}

    //! @name Main
    //@{
    void Update();
    //@}

    //! @name Medical logistic
    //@{
    bool NeedEvacuation(); // NeedMedical() && pas encore pris en charge
    void Evacuate      ( MIL_AutomateLOG& destinationTC2 );
    bool NeedMedical   () const;
    
    void NotifyHandledByMedical ();
    bool NotifyBackToWar        ();
    void HealMentalDisease      ();
    void HealWound              ();
    void HealContamination      ();
    //@}

    //! @name Composante maintenance
    //@{
    void NotifyComposanteHandledByMaintenance();
    void NotifyComposanteBackFromMaintenance ();
    //@}

private:
    //! @name Operators
    //@{
    PHY_Human& operator=( const PHY_Human& rhs );
    //@}

    //! @name Tools
    //@{
    void NotifyHumanChanged( const PHY_Human& oldHumanState );
    //@}

private:
    const MIL_Time_ABC&          time_;
          PHY_HumansComposante*  pComposante_;
    const PHY_HumanRank*         pRank_;
    const PHY_HumanWound*        pWound_;
          bool                   bMentalDiseased_;
          bool                   bContamined_;  
          E_Location             nLocation_;
          PHY_MedicalHumanState* pMedicalState_;
          uint                   nDeathTimeStep_;
};



#include "PHY_Human.inl"

#endif // __PHY_Human_h_
