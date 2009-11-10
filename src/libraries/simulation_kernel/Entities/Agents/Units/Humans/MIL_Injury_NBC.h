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

#include "MIL.h"
#include "MIL_Injury_ABC.h"

#include "Entities/Objects/MIL_MedicalTreatmentType.h"

class PHY_ComposantePion;
class NBCTypeAttribute;


class MIL_Injury_NBC : public MIL_Injury_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    MIL_Injury_NBC();
    explicit MIL_Injury_NBC( NBCTypeAttribute& attr , int injuryID );
    MIL_Injury_NBC( const MIL_Injury_NBC& rhs );
    ~MIL_Injury_NBC();
    //@}

    //! @name Get
    //@{
    int                                          GetInjuryID() const;
    MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const;
    float                                        GetLifeExpectancy( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory ) const;
    //@}

    //! @name Helpers
    //@{
    float SetLifeExpectancy() const;
    bool  IsInjured( const PHY_ComposantePion& pComposante );
    void  SetInjury( MT_Float rNbrAliveHumans , MT_Float rDensity );
    void  SetInjuryCategory( MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory );
    //@}

private:
    //! @name Member data
    //@{
    static MT_Random                             randomGenerator_;
    const NBCTypeAttribute*                          pInjuryAttribute_;
    int                                          injuryID_;
    MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory_;
    //@}
};

#endif // __MIL_Injury_NBC_h_
