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
#include "PHY_HumanFactor.h"

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
class PHY_Experience : public PHY_HumanFactor
                     , private boost::noncopyable
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
          sword::UnitAttributes_EnumUnitExperience GetAsnID                        () const;
    //@}

private:
     PHY_Experience( const std::string& strName, unsigned int nType, sword::UnitAttributes_EnumUnitExperience nAsnID );
    ~PHY_Experience();

private:
    const sword::UnitAttributes_EnumUnitExperience nAsnID_;

private:
    //! @name Statics
    //@{
    static T_ExperienceMap experiences_;
    struct LoadingWrapper;
    static void ReadExperience( xml::xistream& xis );
    //@}
};

#endif // __PHY_Experience_h_
