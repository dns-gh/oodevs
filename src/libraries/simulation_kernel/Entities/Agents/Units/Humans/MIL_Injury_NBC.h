// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/MIL_Injury_NBC.h $
// $Author: RFT $
// $Modtime: 28/04/05 16:04 $
// $Revision: 6 $
// $Workfile: MIL_Injury_NBC.h $
//
// *****************************************************************************

#ifndef __MIL_Injury_NBC_h_
#define __MIL_Injury_NBC_h_

#include "MIL_Injury_ABC.h"

class PHY_ComposantePion;
class NBCTypeAttribute;

class MIL_Injury_NBC : public MIL_Injury_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Injury_NBC();
             MIL_Injury_NBC( NBCTypeAttribute& attr, unsigned int injuryID );
             MIL_Injury_NBC( const MIL_Injury_NBC& rhs );
    virtual ~MIL_Injury_NBC();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int GetInjuryID() const;
    virtual MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const;
    virtual float GetLifeExpectancy( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory ) const;
    //@}

    //! @name Helpers
    //@{
    virtual float SetLifeExpectancy() const;
    virtual bool IsInjured( const PHY_ComposantePion& pComposante );
    virtual void SetInjury( unsigned int nNbrAliveHumans , double rDensity );
    virtual void SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory );
    //@}

private:
    //! @name Member data
    //@{
    const NBCTypeAttribute* pInjuryAttribute_;
    unsigned int injuryID_;
    MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory_;
    //@}
};

//BOOST_CLASS_EXPORT_KEY( MIL_Injury_NBC )

#endif // __MIL_Injury_NBC_h_
