// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/MIL_Injury_Wound.h $
// $Author: RFT $
// $Modtime: 28/04/05 16:04 $
// $Revision: 6 $
// $Workfile: MIL_Injury_Wound.h $
//
// *****************************************************************************

#ifndef __MIL_Injury_Wound_h_
#define __MIL_Injury_Wound_h_

#include "MIL.h"
#include "MIL_Injury_ABC.h"

class PHY_ComposantePion;
class WoundTypeAttribute;
class PHY_InjuredHuman;

class MIL_Injury_Wound : public MIL_Injury_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Injury_Wound();
    explicit MIL_Injury_Wound( unsigned int injuryID );
             MIL_Injury_Wound( const MIL_Injury_Wound& rhs );
    virtual ~MIL_Injury_Wound();
    //@}

public:
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetInjuryID() const;
    virtual MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const;
    virtual float GetLifeExpectancy() const;
    virtual float GetAgentDose()const;
    //@}

    //! @name Helpers
    //@{
    virtual float SetLifeExpectancy() const;
    virtual bool IsInjured( const PHY_ComposantePion& pComposante );
    virtual void SetInjury( unsigned int nNbrAliveHumans, double rDensity );
    virtual void Injure( PHY_InjuredHuman& injuredHuman );
    virtual void SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory );
    virtual bool CanInjuryBeDeadly();
    //@}

    //! @name Update
    //@{
    virtual void UpdateLifeExpectancy( float time );
    virtual void UpdateInjuryCategory();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int injuryID_;
    MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory_;
    float lifeExpectancy_;
    //@}
};

//BOOST_CLASS_EXPORT_KEY( MIL_Injury_Wound )

#endif // __MIL_Injury_Wound_h_
