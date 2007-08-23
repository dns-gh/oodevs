//*****************************************************************************
//
// $Created: JVT 03-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/PHY_Ephemeride.h $
// $Author: Nld $
// $Modtime: 6/01/05 17:25 $
// $Revision: 2 $
// $Workfile: PHY_Ephemeride.h $
//
//*****************************************************************************

#ifndef __PHY_Ephemeride_h_
#define __PHY_Ephemeride_h_

#include "MIL.h"

#include "Meteo/PHY_Lighting.h"

namespace xml
{
    class xistream;
}

//*****************************************************************************
// Created: JVT 03-08-05
// Last modified: JVT 03-08-07
//*****************************************************************************
class PHY_Ephemeride
{
    MT_COPYNOTALLOWED( PHY_Ephemeride )
    
public:
    PHY_Ephemeride( xml::xistream& xis );
    ~PHY_Ephemeride();

    //! @name 
    //@{
    bool                UpdateNight    ();
    const PHY_Lighting& GetLightingBase() const;
    bool                IsNight        () const;
    //@}

private:
    bool                  bIsNight_;
    std::pair< int, int > sunriseTime_;
    std::pair< int, int > sunsetTime_;
    const PHY_Lighting*   pNightBase_;
};

#include "PHY_Ephemeride.inl"

#endif // __PHY_Ephemeride_h_