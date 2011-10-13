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
#include "Human_ABC.h"

class HumansComposante_ABC;
class MIL_Time_ABC;

namespace logistic {
    class FuneralConsign_ABC;
}

//$$$ Clarifier l'interface (trop de trucs différents pour trop de fonctionnalités proches) (Log vs Magic vs attrition)

// =============================================================================
// @class  PHY_Human
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Human : public Human_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_Human( const MIL_Time_ABC& time, HumansComposante_ABC& composante );
             PHY_Human( const PHY_Human& rhs );
             PHY_Human();
    virtual ~PHY_Human();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Heal(); // 'Magic' Heal
    bool SetRank( const PHY_HumanRank&  newRank ); // Force rank change
    bool SetWound( const PHY_HumanWound& newWound ); // Don't test 'usability' or aggravation => force wound change
    bool ApplyWound( const PHY_HumanWound& newWound ); // Test 'usability'
    bool ApplyPoisonous( const MIL_ToxicEffectManipulator& nbcAgent ); // NBC effects
    void ApplyContamination( const MIL_ToxicEffectManipulator& nbcAgent ); // NBC effects
    void ApplyBurn( const MIL_BurnEffectManipulator& burn );
    void ApplyFlood( const MIL_FloodEffectManipulator& flood );
    void ApplyMentalDisease();
    void ForceMentalDisease();
    void CancelLogisticRequests();
    void SetState( const PHY_HumanWound& newWound, bool mentalDisease, bool contaminated );
    //@}

    //! @name Accessors
    //@{
    const PHY_HumanRank& GetRank() const;
    const PHY_HumanWound& GetWound() const;
    E_Location GetLocation() const;
    bool IsUsable() const;
    bool IsDead() const;
    bool IsSeriouslyPhysicallyWounded() const;
    bool IsWounded() const;
    bool IsContaminated() const;
    bool IsMentalDiseased() const;
    bool IsAnEmergency() const;
    const MIL_Agent_ABC& GetPion() const;
    //@}

    //! @name Main
    //@{
    bool NeedUpdate() const;
    void Update();
    void Clean();
    //@}

     //! @name Network
    //@{
    virtual void SendFullState( unsigned int context ) const;
    virtual void SendChangedState() const;
    //@}

    //! @name Medical logistic
    //@{
    bool NeedEvacuation(); // NeedMedical() && pas encore pris en charge
    void Evacuate( MIL_AutomateLOG& destinationTC2 );
    bool NeedMedical() const;
    void SetMedicalState( PHY_MedicalHumanState* pMedicalState );

    void NotifyHandledByMedical();
    bool NotifyBackToWar();
    void HealMentalDisease();
    void HealWound();
    void HealContamination();
    //@}

    //! @name Funeral logistic
    //@{
    virtual void NotifyHandledByFuneral();
    virtual void NotifyBackFromFuneral ();
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
    void NotifyHumanChanged( const Human_ABC& oldHumanState );
    void CancelMedicalLogisticRequest();
    //@}

private:
    const MIL_Time_ABC& time_;
    HumansComposante_ABC* pComposante_;
    const PHY_HumanRank* pRank_;
    const PHY_HumanWound* pWound_;
    bool bMentalDiseased_;
    bool bContamined_;
    E_Location nLocation_;
    PHY_MedicalHumanState* pMedicalState_;
    boost::shared_ptr< logistic::FuneralConsign_ABC > funeralConsign_;
    unsigned int nDeathTimeStep_;
};

BOOST_CLASS_EXPORT_KEY( PHY_Human )

#endif // __PHY_Human_h_
