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

namespace logistic
{
    class FuneralConsign;
}

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
    virtual void Heal(); // 'Magic' Heal
    virtual bool SetRank( const PHY_HumanRank&  newRank ); // Force rank change
    virtual bool SetWound( const PHY_HumanWound& newWound ); // Don't test 'usability' or aggravation => force wound change
    virtual bool ApplyWound( const PHY_HumanWound& newWound ); // Test 'usability'
    virtual void ApplyContamination(); // NBC effects
    virtual void ApplyMentalDisease();
    virtual void ForceMentalDisease();
    virtual void CancelLogisticRequests();
    virtual void SetState( const PHY_HumanWound& newWound, bool mentalDisease, bool contaminated );
    //@}

    //! @name Accessors
    //@{
    virtual const PHY_HumanRank& GetRank() const;
    virtual const PHY_HumanWound& GetWound() const;
    virtual E_HumanLocation GetLocation() const;
    virtual bool IsUsable() const;
    virtual bool IsDead() const;
    virtual bool IsSeriouslyPhysicallyWounded() const;
    virtual bool IsWounded() const;
    virtual bool IsContaminated() const;
    virtual bool IsMentalDiseased() const;
    virtual bool IsAnEmergency() const;
    virtual const MIL_Agent_ABC& GetPion() const;
    virtual bool ShouldGoBackToWar() const;
    //@}

    //! @name Main
    //@{
    virtual bool NeedUpdate() const;
    virtual void Update();
    virtual void Clean();
    //@}

     //! @name Network
    //@{
    virtual void SendFullState( unsigned int context ) const;
    virtual void SendChangedState() const;
    //@}

    //! @name Medical logistic
    //@{
    virtual bool NeedEvacuation() const; // NeedMedical() && pas encore pris en charge
    virtual void Evacuate( MIL_AutomateLOG& destinationTC2 );
    virtual bool NeedMedical() const;
    virtual void SetMedicalState( const boost::shared_ptr< PHY_MedicalHumanState >& medicalState );

    virtual void NotifyHandledByMedical();
    virtual bool NotifyBackToWar();
    virtual void NotifyDiagnosed();
    virtual void HealMentalDisease();
    virtual void HealWound();
    virtual void HealContamination();
    //@}

    //! @name Funeral logistic
    //@{
    virtual void NotifyHandledByFuneral();
    virtual void NotifyBackFromFuneral ();
    //@}

    //! @name Composante maintenance
    //@{
    virtual void NotifyComposanteHandledByMaintenance();
    virtual void NotifyComposanteBackFromMaintenance ();
    //@}

private:
    //! @name Operators
    //@{
    PHY_Human( const PHY_Human& rhs );
    //@}

    //! @name Tools
    //@{
    void NotifyHumanChanged( const Human_ABC& oldHumanState );
    void CancelMedicalLogisticRequest();
    //@}

protected:

    //! @name Tools
    //@{
    virtual bool IsJammed() const;
    //@}

private:
    const MIL_Time_ABC& time_;
    HumansComposante_ABC* pComposante_;
    const PHY_HumanRank* pRank_;
    const PHY_HumanWound* pWound_;
    bool bMentalDiseased_;
    bool bContamined_;
    E_HumanLocation nLocation_;
    boost::shared_ptr< PHY_MedicalHumanState > pMedicalState_;
    boost::shared_ptr< logistic::FuneralConsign > funeralConsign_;
    unsigned int nDeathTimeStep_;
};

BOOST_CLASS_EXPORT_KEY( PHY_Human )

#endif // __PHY_Human_h_
