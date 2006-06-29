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

#include "MIL.h"

#include "MT_Tools/MT_Random.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  PHY_HumanWound
// Created: JVT 2004-08-03
// =============================================================================
class PHY_HumanWound
{
    MT_COPYNOTALLOWED( PHY_HumanWound )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_HumanWound*, sCaseInsensitiveLess > T_HumanWoundMap;
    typedef T_HumanWoundMap::const_iterator                                      CIT_HumanWoundMap;
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
    static void Initialize           ();
    static void InitializeMedicalData( MIL_InputArchive& archive );
    static void Terminate            ();

    static const T_HumanWoundMap& GetHumanWounds();
    static const PHY_HumanWound*  Find( const std::string& strName );
    static const PHY_HumanWound*  Find( ASN1T_EnumHumanWound nAsnID );
    static const PHY_HumanWound*  Find( uint nID );

    static uint GetDiagnosticTime           ();
    static uint GetSortingTime              ();
    static uint GetContaminatedHealingTime  ();
    static uint GetContaminatedRestingTime  ();
    static uint GetMentalDiseaseHealingTime ();
    static uint GetMentalDiseaseRestingTime ();
    static bool ChooseMentalDisease         ();

    //@}

    //! @name Accessors
    //@{
    const std::string&   GetName          () const;
    uint                 GetID            () const;
    ASN1T_EnumHumanWound GetAsnID         () const;
    MT_Float             GetWoundedFactor () const;
    uint                 GetLifeExpectancy() const;
    uint                 GetHealingTime   () const;
    uint                 GetRestingTime   () const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_HumanWound& rhs ) const;
    bool operator!=( const PHY_HumanWound& rhs ) const;
    //@}

    //! @name Operations
    //@{
    const PHY_HumanWound& Aggravate() const;
    const PHY_HumanWound& Aggravate( const PHY_HumanWound& degradation ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Wound
    {
        eNotWounded, 
        eKilled,
        eWoundedU1,
        eWoundedU2,
        eWoundedU3,
        eWoundedUE
    };

    typedef std::vector< MT_Float >       T_FloatVector;
    typedef T_FloatVector::const_iterator CIT_FloatVector;
    //@}

private:
     PHY_HumanWound( const std::string& strName, E_Wound nWound, const PHY_HumanWound* pPrevious, const PHY_HumanWound* pNext, const ASN1T_EnumHumanWound& nAsnID );
    ~PHY_HumanWound();

    //! @name Init
    //@{
    static void InitializeWoundedInEquipements( MT_InputArchive_ABC& archive );
    static void InitializeWoundedCoefs        ( MT_InputArchive_ABC& archive );
    //@}

    //! @name Tools
    //@{
    static const PHY_HumanWound& GetRandomWound();
    //@}

private:
    const std::string           strName_;
    const E_Wound               nWound_;
    const ASN1T_EnumHumanWound  nAsnID_;
    const PHY_HumanWound*       pPrevious_;
    const PHY_HumanWound*       pNext_;
          MT_Float              rWoundedFactor_;
          uint                  nLifeExpectancy_;
          uint                  nHealingTime_;
          uint                  nRestingTime_;

private:
    static T_HumanWoundMap humanWounds_;
    static MT_Random       randomGenerator_;
    static uint            nDiagnosticTime_;
    static uint            nSortingTime_;

    static uint            nContaminatedHealingTime_;
    static uint            nContaminatedRestingTime_;
    static uint            nMentalDiseaseHealingTime_;
	static uint	           nMentalDiseaseRestingTime_;
    static MT_Float        rMentalDiseaseFactor_;
};

#include "PHY_HumanWound.inl"

#endif // __PHY_HumanWound_h_
