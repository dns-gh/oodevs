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

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/split_member.hpp>
#include <utility>

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

namespace weather
{
    class PHY_Lighting;
}

namespace xml
{
    class xistream;
    class xostream;
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
             PHY_Ephemeride( const std::string& dayBase, const std::string& nightBase,
                const std::string& sunrise, const std::string& sunset,
                uint32_t epochTime );
    virtual ~PHY_Ephemeride();
    //@}

    //! @name Operations
    //@{
    bool UpdateNight( uint32_t date );
    const weather::PHY_Lighting& GetLightingBase() const;
    bool IsNight() const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteUrban( xml::xostream& xos );
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

boost::shared_ptr< PHY_Ephemeride > ReadEphemeride(
        xml::xistream& xis, uint32_t epochTime );


#endif // __PHY_Ephemeride_h_
