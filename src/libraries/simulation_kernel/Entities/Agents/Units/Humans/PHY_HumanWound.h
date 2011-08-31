// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumanWound.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:09 $
// $Revision: 2 $
// $Workfile: PHY_HumanWound.h $
//
// *****************************************************************************

#ifndef __PHY_HumanWound_h_
#define __PHY_HumanWound_h_

#include "MT_Tools/MT_String.h"

namespace sword
{
    enum EnumHumanWound;
}

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_HumanWound
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumanWound : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_HumanWound*, sCaseInsensitiveLess > T_HumanWoundMap;
    typedef T_HumanWoundMap::const_iterator                                    CIT_HumanWoundMap;
    //@}

public:
    static const PHY_HumanWound woundedU3_;
    static const PHY_HumanWound woundedU2_;
    static const PHY_HumanWound woundedU1_;
    static const PHY_HumanWound woundedUE_;
    static const PHY_HumanWound notWounded_;
    static const PHY_HumanWound killed_;

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void InitializeMedicalData( xml::xistream& xis );
    static void Terminate();

    static const T_HumanWoundMap& GetHumanWounds();
    static const PHY_HumanWound* Find( const std::string& strName );
    static const PHY_HumanWound* Find( sword::EnumHumanWound nAsnID );
    static const PHY_HumanWound* Find( unsigned int nID );

    static unsigned int GetDiagnosticTime();
    static unsigned int GetSortingTime();
    static unsigned int GetContaminatedHealingTime();
    static unsigned int GetContaminatedRestingTime();
    static unsigned int GetMentalDiseaseHealingTime();
    static unsigned int GetMentalDiseaseRestingTime();
    static bool ChooseMentalDisease();
    static const PHY_HumanWound& ChooseRandomWound();
    static const PHY_HumanWound* GetRandomWoundSeriousness();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    unsigned int GetID() const;
    sword::EnumHumanWound GetAsnID() const;
    double GetWoundedFactor() const;
    unsigned int GetLifeExpectancy() const;
    unsigned int GetHealingTime() const;
    unsigned int GetRestingTime() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_HumanWound& rhs ) const;
    bool operator!=( const PHY_HumanWound& rhs ) const;
    bool operator< ( const PHY_HumanWound& rhs ) const;
    bool operator> ( const PHY_HumanWound& rhs ) const;
    //@}

    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadInjury( xml::xistream& xis, double& rFactorSum );
    //@}

private:
    //! @name Types
    //@{
    enum E_Wound
    {
        eNotWounded,
        eWoundedU3,
        eWoundedU2,
        eWoundedU1,
        eWoundedUE,
        eKilled
    };

    typedef std::vector< double >           T_FloatVector;
    typedef T_FloatVector::const_iterator CIT_FloatVector;
    //@}

private:
     PHY_HumanWound( const std::string& strName, E_Wound nWound, const sword::EnumHumanWound& nAsnID );
    ~PHY_HumanWound();

    //! @name Init
    //@{
    static void InitializeWoundedInEquipements( xml::xistream& xis );
    static void InitializeWoundedCoefs        ( xml::xistream& xis );
    //@}

private:
    const std::string strName_;
    const E_Wound nWound_;
    const sword::EnumHumanWound  nAsnID_;
    double rWoundedFactor_;
    unsigned int nLifeExpectancy_;
    unsigned int nHealingTime_;
    unsigned int nRestingTime_;

private:
    static T_HumanWoundMap humanWounds_;
    static unsigned int nDiagnosticTime_;
    static unsigned int nSortingTime_;
    static unsigned int nContaminatedHealingTime_;
    static unsigned int nContaminatedRestingTime_;
    static unsigned int nMentalDiseaseHealingTime_;
    static unsigned int nMentalDiseaseRestingTime_;
    static double rMentalDiseaseFactor_;
};

#endif // __PHY_HumanWound_h_
