// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_Stress_h_
#define __PHY_Stress_h_

#include "MT_Tools/MT_String.h"
#include "PHY_HumanFactor.h"


namespace sword
{
    enum UnitAttributes_EnumUnitStress;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  PHY_Stress
    @brief  PHY_Stress
*/
// Created: LDC 2011-06-17
// =============================================================================
class PHY_Stress : public PHY_HumanFactor
                 , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Stress* > T_StressMap;
    typedef T_StressMap::const_iterator                CIT_StressMap;

    struct Evolution
    {
        Evolution() : firstThreshold_( 0.f ), secondThreshold_( 0.f ), incPerShot_( 0.f ), decPerHour_( 0.f ) {};

        double firstThreshold_;
        double secondThreshold_;
        double incPerShot_;
        double decPerHour_;
    };
    //@}

public:
    //! @name Constants
    //@{
    static const PHY_Stress calm_;
    static const PHY_Stress worried_;
    static const PHY_Stress stressed_;
    static Evolution evolution_;
    //@}

public:
    //! @name Manager
    //@{
    static       void        Initialize( xml::xistream& xis );
    static       void        Terminate ();
    static const PHY_Stress* Find      ( const std::string& strName );
    static const PHY_Stress* Find      ( sword::UnitAttributes_EnumUnitStress nAsnID );
    static const PHY_Stress* Find      ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    sword::UnitAttributes_EnumUnitStress GetAsnID    () const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    PHY_Stress( const std::string& strName, unsigned int nType, sword::UnitAttributes_EnumUnitStress nAsnID );
    ~PHY_Stress();
    //@}

private:
    //! @name Static Helpers
    //@{
    struct LoadingWrapper;
    static void ReadStress( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const sword::UnitAttributes_EnumUnitStress nAsnID_;
    //@}

private:
    //! @name Static member data
    //@{
    static T_StressMap stresses_;
    //@}
};

#endif // __PHY_Stress_h_
