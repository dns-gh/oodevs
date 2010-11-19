// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_InjuredHuman.h $
// $Author: RFT $
// $Modtime: 28/04/05 16:04 $
// $Revision: 6 $
// $Workfile: PHY_InjuredHuman.h $
//
// *****************************************************************************

#ifndef __PHY_InjuredHuman_h_
#define __PHY_InjuredHuman_h_

#include "MIL.h"
#include "Entities/Objects/MIL_MedicalTreatmentType.h"

class MIL_Injury_ABC;
class PHY_ComposantePion;
class MedicalTreatmentAttribute;

class PHY_InjuredHuman
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_InjuredHuman();
             PHY_InjuredHuman( MIL_Injury_ABC& injury , const PHY_ComposantePion& composantePion );
    explicit PHY_InjuredHuman( MIL_Injury_ABC& injury );
             PHY_InjuredHuman( const PHY_InjuredHuman& rhs );
    virtual ~PHY_InjuredHuman();
    //@}

public:
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

public:
    //! @name Operations
    //@{
    static void InitializeInjuredHuman( MIL_Injury_ABC& injury , const PHY_ComposantePion& composantePion );
    static void InitializeInjuredHuman( MIL_Injury_ABC& injury );
    //@}

public:
    //! @name Accessors
    //@{
    int GetLifeExpectancy() const;
    MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const;
    MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory( unsigned int injuryID ) const;
    unsigned int GetInjuryID() const;
    const PHY_ComposantePion* GetComposantePion() const;
    //@}

public:
    //! @name Update
    //@{
    void Update();
    void UpdateInjuriesInfo( float currentTime );
    void UpdateInjuredHumanInfo( float currentTime );
    //@}

    //! @name Helpers
    bool IsAlive() const;
    void AddInjury( MIL_Injury_ABC& injury );
    void TreatInjuredHuman( MedicalTreatmentAttribute& attr );
    bool FindInjury( unsigned int injuryID );
    //@}

private:
    //! @name Types
    //@{
    typedef std::list< MIL_Injury_ABC* >     InjuriesList;
    typedef InjuriesList::iterator        IT_InjuriesList;
    typedef InjuriesList::const_iterator CIT_InjuriesList;
    //@}

private:
    //! @name Member data
    //@{
    float lifeExpectancy_;
    MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory_;
    unsigned int injuryID_;
    bool isTreated_;
    InjuriesList injuriesList_;
    const PHY_ComposantePion* pComposantePion_;
    float timeOfLastUpdate_;
    MedicalTreatmentAttribute* treatment_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_InjuredHuman )

#endif // __PHY_InjuredHuman_h_
