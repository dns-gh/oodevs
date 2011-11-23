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

namespace weather
{
    class PHY_Lighting;
}

namespace xml
{
    class xistream;
}

//*****************************************************************************
// Created: JVT 03-08-05
// Last modified: JVT 03-08-07
//*****************************************************************************
class PHY_Ephemeride : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_Ephemeride();
    explicit PHY_Ephemeride( xml::xistream& xis );
    virtual ~PHY_Ephemeride();
    //@}

    //! @name Operations
    //@{
    bool UpdateNight( unsigned int date );
    const weather::PHY_Lighting& GetLightingBase() const;
    bool IsNight() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool bIsNight_;
    std::pair< int, int > sunriseTime_;
    std::pair< int, int > sunsetTime_;
    const weather::PHY_Lighting* pDayBase_;
    const weather::PHY_Lighting* pNightBase_;
    //@}
};

#include "PHY_Ephemeride.inl"

#endif // __PHY_Ephemeride_h_
