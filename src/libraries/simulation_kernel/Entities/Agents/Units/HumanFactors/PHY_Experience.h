// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/HumanFactors/PHY_Experience.h $
// $Author: Jvt $
// $Modtime: 14/04/05 16:28 $
// $Revision: 4 $
// $Workfile: PHY_Experience.h $
//
// *****************************************************************************

#ifndef __PHY_Experience_h_
#define __PHY_Experience_h_

#include "MIL.h"

#include "game_asn/Simulation.h"

namespace xml
{
    class xistream;
}

class PHY_ConsumptionType;

// =============================================================================
// @class  PHY_Experience
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Experience
{
    MT_COPYNOTALLOWED( PHY_Experience )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Experience*, sCaseInsensitiveLess > T_ExperienceMap;
    typedef T_ExperienceMap::const_iterator                                      CIT_ExperienceMap;
    //@}

public:
    //! @name 
    //@{
    static const PHY_Experience conscrit_;
    static const PHY_Experience experimente_;
    static const PHY_Experience veteran_;
    //@}

public:
    //! @name Manager
    //@{
    static       void            Initialize( xml::xistream& xis );
    static       void            Terminate ();
    static const PHY_Experience* Find      ( const std::string& strName );
    static const PHY_Experience* Find      ( ASN1T_EnumUnitExperience nAsnID );
    static const PHY_Experience* Find      ( uint nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&             GetName                         () const;
          uint                     GetID                           () const;
          ASN1T_EnumUnitExperience GetAsnID                        () const;
          MT_Float                 GetWeight                       () const;
          MT_Float                 GetCoefMaxSpeedModificator      () const;
          MT_Float                 GetCoefReloadingTimeModificator () const;
          MT_Float                 GetCoefPhModificator            () const;
          MT_Float                 GetCoefPostureTimeModificator   () const;
          MT_Float                 GetCoefSensorDistanceModificator() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_Experience& rhs ) const;
    bool operator!=( const PHY_Experience& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ExperienceType
    {
        eVeteran     = 0,
        eExperimente = 1,
        eConscrit    = 2
    };
    //@}

private:
     PHY_Experience( const std::string& strName, E_ExperienceType nType, ASN1T_EnumUnitExperience nAsnID, MT_Float rDIAWeight );
    ~PHY_Experience();

    //! @name Init
    //@{
    void Read( xml::xistream& xis );
    //@}

private:
    const std::string              strName_;
    const E_ExperienceType         nType_;
    const ASN1T_EnumUnitExperience nAsnID_;
    const MT_Float                 rDIAWeight_;

    MT_Float rCoefMaxSpeedModificator_;
    MT_Float rCoefReloadingTimeModificator_;
    MT_Float rCoefPhModificator_;
    MT_Float rCoefPostureTimeModificator_;
    MT_Float rCoefSensorDistanceModificator_;

private:
    static T_ExperienceMap experiences_;
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadExperience( xml::xistream& xis );
    //@}
};

#include "PHY_Experience.inl"

#endif // __PHY_Experience_h_
