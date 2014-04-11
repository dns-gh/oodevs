// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __weather_MeteoLocal_h_
#define __weather_MeteoLocal_h_

#include "Meteo.h"
#include <boost/date_time.hpp>

namespace sword
{
    class ControlLocalWeatherCreation;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace weather
{

// =============================================================================
/** @class  MeteoLocal
    @brief  MeteoLocal
*/
// Created: ABR 2011-06-06
// =============================================================================
class MeteoLocal : public Meteo
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MeteoLocal( const MeteoLocal& ); //!< Copy constructor
    explicit MeteoLocal( const kernel::CoordinateConverter_ABC& converterr, const std::string& name = "" );
             MeteoLocal( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const std::string& name /*= ""*/ );
             MeteoLocal( const sword::ControlLocalWeatherCreation& msg, const kernel::CoordinateConverter_ABC& converter, unsigned int timeStep, const std::string& name /*= ""*/ );
             MeteoLocal( const sword::ControlLocalWeatherCreation& msg, unsigned int timeStep, const std::string& name /*= ""*/ );
    virtual ~MeteoLocal();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    void SetPosition( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight );
    void SetPeriod( const boost::posix_time::ptime& start, const boost::posix_time::ptime& end );
    void SetCreated( bool created );

    const geometry::Point2f GetTopLeft() const;
    const geometry::Point2f GetBottomRight() const;
    const boost::posix_time::ptime& GetStartTime() const;
    const boost::posix_time::ptime& GetEndTime() const;
    bool IsCreated() const;
    const kernel::CoordinateConverter_ABC& GetCoordinateConverter() const;

    virtual bool IsInside( const geometry::Point2f& point ) const;
    bool IsValid() const;
    void Update( const sword::ControlLocalWeatherCreation& msg );
    virtual void SendCreation( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateRectangle();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MeteoLocal& operator=( const MeteoLocal ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC* converter_;
    geometry::Point2f topLeft_;
    geometry::Point2f bottomRight_;
    boost::posix_time::ptime startTime_;
    boost::posix_time::ptime endTime_;
    bool created_;
    //@}

public:
    //! @name Static member data
    //@{
    static unsigned int localCounter_;
    //@}
};

}

#endif // __weather_MeteoLocal_h_
