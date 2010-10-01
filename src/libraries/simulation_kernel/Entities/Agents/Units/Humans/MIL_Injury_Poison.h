// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/MIL_Injury_Poison.h $
// $Author: RFT $
// $Modtime: 28/04/05 16:04 $
// $Revision: 6 $
// $Workfile: MIL_Injury_Poison.h $
//
// *****************************************************************************

#ifndef __MIL_Injury_Poison_h_
#define __MIL_Injury_Poison_h_

#include "MIL.h"
#include "MIL_Injury_ABC.h"

#include "Entities/Objects/MIL_MedicalTreatmentType.h"

class PHY_ComposantePion;
class NBCTypeAttribute;
class PHY_InjuredHuman;


class MIL_Injury_Poison : public MIL_Injury_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Injury_Poison();
    explicit MIL_Injury_Poison( int agentConcentration, const std::string& NBCAgent, int injuryID );
    //@}

    //! @name
    //@{
    MIL_Injury_Poison( const MIL_Injury_Poison& rhs );
    virtual ~MIL_Injury_Poison();
    //@}

public:
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Get
    //@{
    int                                          GetInjuryID() const;
    MIL_MedicalTreatmentType::E_InjuryCategories GetInjuryCategory() const;
    float                                        GetLifeExpectancy() const;
    float                                        GetAgentDose()const;
    //@}

    //! @name Helpers
    //@{
    float SetLifeExpectancy() const;
    bool  IsInjured( const PHY_ComposantePion& pComposante );
    void  SetInjury( unsigned int nNbrAliveHumans , MT_Float rDensity );
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
    int                                          agentConcentration_;
    const std::string                            NBCAgent_;
    int                                          injuryID_;
    MIL_MedicalTreatmentType::E_InjuryCategories injuryCategory_;
    float                                        lifeExpectancy_;
    //@}
};

//BOOST_CLASS_EXPORT_KEY( MIL_Injury_Poison )

#endif // __MIL_Injury_Poison_h_
