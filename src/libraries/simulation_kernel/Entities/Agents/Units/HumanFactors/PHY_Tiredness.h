// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/HumanFactors/PHY_Tiredness.h $
// $Author: Jvt $
// $Modtime: 14/04/05 16:30 $
// $Revision: 4 $
// $Workfile: PHY_Tiredness.h $
//
// *****************************************************************************

#ifndef __PHY_Tiredness_h_
#define __PHY_Tiredness_h_

#include "MT_Tools/MT_String.h"
#include "PHY_HumanFactor.h"

namespace sword
{
    enum UnitAttributes_EnumUnitTiredness;
}

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_Tiredness
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Tiredness : public PHY_HumanFactor
                    , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Tiredness* > T_TirednessMap;
    typedef T_TirednessMap::const_iterator              CIT_TirednessMap;

    struct Evolution
    {
        Evolution() : firstThreshold_( 0.f ), secondThreshold_( 0.f ), engineRunning_( 0.f ), engineStopped_( 0.f ), moving_( 0.f ), working_( 0.f ) {};

        double firstThreshold_;
        double secondThreshold_;
        double engineRunning_;
        double engineStopped_;
        double moving_;
        double working_;
    };
    //@}

public:
    //! @name
    //@{
    static const PHY_Tiredness normal_;
    static const PHY_Tiredness fatigue_;
    static const PHY_Tiredness epuise_;
    static Evolution evolution_;
    //@}

public:
    //! @name Manager
    //@{
    static       void           Initialize( xml::xistream& xis );
    static       void           Terminate ();
    static const PHY_Tiredness* Find      ( const std::string& strName );
    static const PHY_Tiredness* Find      ( sword::UnitAttributes_EnumUnitTiredness nAsnID );
    static const PHY_Tiredness* Find      ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    sword::UnitAttributes_EnumUnitTiredness GetAsnID    () const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    PHY_Tiredness( const std::string& strName, unsigned int nType, sword::UnitAttributes_EnumUnitTiredness nAsnID );
    ~PHY_Tiredness();
    //@}

private:
    //! @name Static helpers
    //@{
    struct LoadingWrapper;
    static void ReadTiredness( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const sword::UnitAttributes_EnumUnitTiredness nAsnID_;
    //@}

private:
    //! @name Statics member data
    //@{
    static T_TirednessMap tirednesses_;
    //@}
};

#endif // __PHY_Tiredness_h_
