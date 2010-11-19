// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Injury_ABC_h_
#define __MIL_Injury_ABC_h_

#include "Entities/Objects/MIL_MedicalTreatmentType.h"

class PHY_ComposantePion;
class PHY_InjuredHuman;

class MIL_Injury_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Injury_ABC() {}
    virtual ~MIL_Injury_ABC() {}
    //@}

public:
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetInjuryID() const = 0;
    virtual MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const = 0;
    virtual float GetLifeExpectancy() const = 0;
    virtual float GetAgentDose() const = 0;
    //@}

    //! @name Helpers
    //@{
    virtual float SetLifeExpectancy() const = 0;
    virtual bool IsInjured( const PHY_ComposantePion& pComposante ) = 0; //This is used by the "ComposantePion"
    virtual void SetInjury( unsigned int nNbrAliveHumans, double rDensity ) = 0; //This is used by the population
    virtual void Injure( PHY_InjuredHuman& injuredHuman ) = 0; //This is used by someone already injured
    virtual void SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory ) = 0;
    virtual bool CanInjuryBeDeadly() = 0;
    //@}

    //! @name Update
    //@{
    virtual void UpdateLifeExpectancy( float time ) = 0;
    virtual void UpdateInjuryCategory() = 0;
    //@}
};

#endif // __MIL_Injury_ABC_h_
