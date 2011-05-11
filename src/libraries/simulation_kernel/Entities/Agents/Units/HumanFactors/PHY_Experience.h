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

#include "MT_Tools/MT_String.h"

namespace sword
{
    enum UnitAttributes_EnumUnitExperience;
}

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_Experience
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Experience : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Experience* > T_ExperienceMap;
    typedef T_ExperienceMap::const_iterator              CIT_ExperienceMap;
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
    static const PHY_Experience* Find      ( sword::UnitAttributes_EnumUnitExperience nAsnID );
    static const PHY_Experience* Find      ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string&             GetName                         () const;
          unsigned int                     GetID                           () const;
          sword::UnitAttributes_EnumUnitExperience GetAsnID                        () const;
          double                 GetWeight                       () const;
          double                 GetCoefMaxSpeedModificator      () const;
          double                 GetCoefReloadingTimeModificator () const;
          double                 GetCoefPhModificator            () const;
          double                 GetCoefPostureTimeModificator   () const;
          double                 GetCoefSensorDistanceModificator() const;
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
     PHY_Experience( const std::string& strName, E_ExperienceType nType, sword::UnitAttributes_EnumUnitExperience nAsnID, double rDIAWeight );
    ~PHY_Experience();

    //! @name Init
    //@{
    void Read( xml::xistream& xis );
    //@}

private:
    const std::string              strName_;
    const E_ExperienceType         nType_;
    const sword::UnitAttributes_EnumUnitExperience nAsnID_;
    const double                 rDIAWeight_;

    double rCoefMaxSpeedModificator_;
    double rCoefReloadingTimeModificator_;
    double rCoefPhModificator_;
    double rCoefPostureTimeModificator_;
    double rCoefSensorDistanceModificator_;

private:
    //! @name Statics
    //@{
    static T_ExperienceMap experiences_;
    struct LoadingWrapper;
    static void ReadExperience( xml::xistream& xis );
    //@}
};

#endif // __PHY_Experience_h_
