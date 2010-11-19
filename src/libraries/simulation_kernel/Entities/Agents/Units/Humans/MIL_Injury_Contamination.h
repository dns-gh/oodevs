// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Injury_Contamination_h_
#define __MIL_Injury_Contamination_h_

#include "MIL.h"
#include "MIL_Injury_ABC.h"

class PHY_ComposantePion;
class PHY_InjuredHuman;

class MIL_Injury_Contamination : public MIL_Injury_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Injury_Contamination();
             MIL_Injury_Contamination( int agentConcentration, const std::string& NBCAgent, unsigned int injuryID );
             MIL_Injury_Contamination( const MIL_Injury_Contamination& rhs );
    virtual ~MIL_Injury_Contamination();
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
    virtual float GetAgentDose() const;
    //@}

    //! @name Helpers
    //@{
    virtual float SetLifeExpectancy() const;
    virtual bool IsInjured( const PHY_ComposantePion& pComposante );
    virtual void SetInjury( unsigned int rNbrAliveHumans, double rDensity );
    virtual void Injure( PHY_InjuredHuman& injuredHuman );
    virtual void SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory );
    virtual bool CanInjuryBeDeadly();
    //@}

    //! @name Update
    //@{
    virtual void UpdateLifeExpectancy( float time ) ;
    virtual void UpdateInjuryCategory();
    //@}

private:
    //! @name Member data
    //@{
    int agentConcentration_;
    const std::string NBCAgent_;
    unsigned int injuryID_;
    MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory_;
    float agentDose_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_Injury_Contamination )

#endif // __MIL_Injury_Contamination_h_
