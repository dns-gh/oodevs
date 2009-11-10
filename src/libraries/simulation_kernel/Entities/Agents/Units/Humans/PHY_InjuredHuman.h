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

class PHY_InjuredHuman
{
private:
    typedef std::list< MIL_Injury_ABC *>    InjuriesList;
    typedef InjuriesList::const_iterator    CIT_InjuriesList;
    typedef InjuriesList::iterator          IT_InjuriesList;

public:
    //! @name Constructors/Destructor
    //@{
             PHY_InjuredHuman();    
             PHY_InjuredHuman( MIL_Injury_ABC& injury , const PHY_ComposantePion& composantePion );
    explicit PHY_InjuredHuman( MIL_Injury_ABC& injury );
    virtual ~PHY_InjuredHuman();
    //@}

    //! @name 
    //@{
    PHY_InjuredHuman( const PHY_InjuredHuman& rhs );
    //@}

public:
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

public:
    static void InitializeInjuredHuman( MIL_Injury_ABC& injury , const PHY_ComposantePion& composantePion );
    static void InitializeInjuredHuman( MIL_Injury_ABC& injury );

public:
    //! @name Get
    //@{
    int                                          GetLifeExpectancy() const;
    MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const;
    MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory( int injuryID ) const;
    int                                          GetInjuryID() const;
    const PHY_ComposantePion*                    GetComposantePion() const;
    //@}

public:
    //! @name Update
    //@{
    void Update();
    void UpdateInjuriesInfo( float currentTime );
    void UpdateInjuredHumanInfo( float currentTime );
    //@}

    //! @nameHelpers
    bool IsAlive();
    void AddInjury( MIL_Injury_ABC& injury );
    void TreatInjuredHuman();
    bool FindInjury( int injuryID );
    //@}

private:
    //! @name Member data
    //@{
    float                                           lifeExpectancy_;
    MIL_MedicalTreatmentType::E_InjuryCategories    injuryCategory_;
    int                                             injuryID_;
    bool                                            isTreated_;
    InjuriesList                                    injuriesList_;
    const PHY_ComposantePion*                       pComposantePion_;
    float                                           timeOfLastUpdate_;
    //@}
};

#endif // __PHY_InjuredHuman_h_
