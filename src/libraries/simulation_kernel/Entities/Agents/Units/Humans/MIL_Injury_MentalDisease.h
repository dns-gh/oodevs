// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/MIL_Injury_MentalDisease.h $
// $Author: RFT $
// $Modtime: 28/04/05 16:04 $
// $Revision: 6 $
// $Workfile: MIL_Injury_MentalDisease.h $
//
// *****************************************************************************

#ifndef __MIL_Injury_MentalDisease_h_
#define __MIL_Injury_MentalDisease_h_

#include "MIL.h"
#include "MIL_Injury_ABC.h"

#include "Entities/Objects/MIL_MedicalTreatmentType.h"

class PHY_ComposantePion;
class FireAttribute;
class PHY_InjuredHuman;


class MIL_Injury_MentalDisease : public MIL_Injury_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    MIL_Injury_MentalDisease();
    explicit MIL_Injury_MentalDisease( int injuryID );
    MIL_Injury_MentalDisease( const MIL_Injury_MentalDisease& rhs );
    ~MIL_Injury_MentalDisease();
    //@}

public:
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Get
    //@{
    int                                          GetInjuryID() const;
    MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const;
    float                                        GetLifeExpectancy() const;
    float                                        GetAgentDose() const;
    //@}

    //! @name Helpers
    //@{
    float SetLifeExpectancy() const;
    //This function is used by pion and their composantes to set the injury
    bool  IsInjured( const PHY_ComposantePion& pComposante );
    //This function is used only by the population to set the injury
    void  SetInjury( MT_Float rNbrAliveHumans , MT_Float rDensity );
    void  Injure( PHY_InjuredHuman& injuredHuman );
    void  SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory );
    bool  CanInjuryBeDeadly();
    //@}

    //! @name Update
    //@{
    void  UpdateLifeExpectancy( float time ) ;
    void  UpdateInjuryCategory();
    //@}

private:
    //! @name Member data
    //@{
    static MT_Random                             randomGenerator_;
    int                                          injuryID_;
    MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory_;
    //@}
};

#endif // __MIL_Injury_MentalDisease_h_
