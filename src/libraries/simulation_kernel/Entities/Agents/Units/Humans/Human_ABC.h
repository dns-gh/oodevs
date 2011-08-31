// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Human_ABC_h_
#define __Human_ABC_h_

class MIL_AutomateLOG;
class MIL_ToxicEffectManipulator;
class MIL_BurnEffectManipulator;
class MIL_FloodEffectManipulator;
class PHY_HumanRank;
class PHY_HumanWound;
class PHY_MedicalHumanState;

// =============================================================================
// @class  Human_ABC
// Created: MGD 2010-03-17
// =============================================================================
class Human_ABC : private boost::noncopyable
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

public:
    //! @name Constructors/Destructor
    //@{
             Human_ABC(){};
    virtual ~Human_ABC(){};
    //@}

    //! @name Operations
    //@{
    virtual void Heal() = 0; // 'Magic' Heal
    virtual bool SetRank( const PHY_HumanRank&  newRank  ) = 0; // Force rank change
    virtual bool SetWound( const PHY_HumanWound& newWound ) = 0; // Don't test 'usability' or aggravation => force wound change
    virtual bool ApplyWound( const PHY_HumanWound& newWound ) = 0; // Test 'usability'
    virtual bool ApplyPoisonous( const MIL_ToxicEffectManipulator& nbcAgent ) = 0; // NBC effects
    virtual void ApplyContamination( const MIL_ToxicEffectManipulator& nbcAgent ) = 0; // NBC effects
    virtual void ApplyBurn( const MIL_BurnEffectManipulator& burn ) = 0;
    virtual void ApplyFlood( const MIL_FloodEffectManipulator& flood ) = 0;
    virtual void ApplyMentalDisease() = 0;
    virtual void ForceMentalDisease() = 0;
    virtual void CancelLogisticRequest() = 0;
    virtual void SetState( const PHY_HumanWound& newWound, bool mentalDisease, bool contaminated ) = 0;
    //@}*/

    //! @name Accessors
    //@{
    virtual const PHY_HumanRank& GetRank() const = 0;
    virtual const PHY_HumanWound& GetWound() const = 0;
    virtual E_Location GetLocation() const = 0;
    virtual bool IsUsable() const = 0;
    virtual bool IsDead() const = 0;
    virtual bool IsWounded() const = 0;
    virtual bool IsContaminated() const = 0;
    virtual bool IsMentalDiseased() const = 0;
    virtual bool IsAnEmergency() const = 0;
    //@}

    //! @name Main
    //@{
    virtual void Update() = 0;
    //@}

    //! @name Medical logistic
    //@{
    virtual bool NeedEvacuation() = 0; // NeedMedical() && pas encore pris en charge
    virtual void Evacuate( MIL_AutomateLOG& destinationTC2 ) = 0;
    virtual bool NeedMedical() const = 0;
    virtual void SetMedicalState( PHY_MedicalHumanState* pMedicalState ) = 0;

    virtual void NotifyHandledByMedical() = 0;
    virtual bool NotifyBackToWar() = 0;
    virtual void HealMentalDisease() = 0;
    virtual void HealWound() = 0;
    virtual void HealContamination() = 0;
    //@}

    //! @name Composante maintenance
    //@{
    virtual void NotifyComposanteHandledByMaintenance() = 0;
    virtual void NotifyComposanteBackFromMaintenance () = 0;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
};

#endif // __Human_ABC_h_
