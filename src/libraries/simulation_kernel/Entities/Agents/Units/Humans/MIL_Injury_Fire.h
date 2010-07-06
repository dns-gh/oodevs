// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Injury_Fire_h_
#define __MIL_Injury_Fire_h_

#include "MIL.h"
#include "MIL_Injury_ABC.h"

#include "Entities/Objects/MIL_MedicalTreatmentType.h"

class PHY_ComposantePion;
class FireAttribute;

class MIL_Injury_Fire : public MIL_Injury_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Injury_Fire();
    explicit MIL_Injury_Fire( int heat, const std::string fireAgent, int injuryID );
             MIL_Injury_Fire( const MIL_Injury_Fire& rhs );
    virtual ~MIL_Injury_Fire();
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
    void  SetInjury( unsigned int nNbrAliveHumans , MT_Float rDensity );
    //This function is used to injure humans that are already injured
    void  Injure( PHY_InjuredHuman& injuredHuman );
    void  SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory );
    bool  CanInjuryBeDeadly();
    //@}

    //! @name Update
    //@{
    void  UpdateLifeExpectancy( float time );
    void  UpdateInjuryCategory();
    //@}

private:
    //! @name Member data
    //@{
    int                                          heat_;
    const std::string                            fireClass_;
    int                                          injuryID_;
    MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory_;
    float                                        lifeExpectancy_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_Injury_Fire )

#endif // __MIL_Injury_Fire_h_
