// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/MIL_Injury_ABC.h $
// $Author: RFT $
// $Modtime: 28/04/05 16:04 $
// $Revision: 6 $
// $Workfile: MIL_Injury_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_Injury_ABC_h_
#define __MIL_Injury_ABC_h_

#include "MIL.h"
#include "Entities/Objects/MIL_MedicalTreatmentType.h"

class PHY_ComposantePion;
class PHY_InjuredHuman;

class MIL_Injury_ABC
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
    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}

    //! @name Get
    //@{
    virtual int                                          GetInjuryID() const = 0;
    virtual MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const = 0;
    virtual float                                        GetLifeExpectancy() const = 0;
    virtual float                                        GetAgentDose() const = 0;
    //@}

    //! @name Helpers
    //@{
    virtual float SetLifeExpectancy() const = 0;
    virtual bool  IsInjured( const PHY_ComposantePion& pComposante ) = 0; //This is used by the "ComposantePion"
    virtual void  SetInjury( MT_Float rNbrAliveHumans, MT_Float rDensity ) = 0; //This is used by the population
    virtual void  Injure( PHY_InjuredHuman& injuredHuman ) = 0; //This is used by someone already injured
    virtual void  SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory ) = 0;
    virtual bool  CanInjuryBeDeadly() = 0;
    //@}

    //! @name Update
    //@{
    virtual void  UpdateLifeExpectancy( float time ) = 0;
    virtual void  UpdateInjuryCategory() = 0;
    //@}
};

#endif // __MIL_Injury_ABC_h_
