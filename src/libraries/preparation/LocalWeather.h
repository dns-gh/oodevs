// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LocalWeather_h_
#define __LocalWeather_h_

#include "Weather.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  LocalWeather
    @brief  Local weather
*/
// Created: SBO 2006-12-20
// =============================================================================
class LocalWeather : public Weather
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LocalWeather( const kernel::CoordinateConverter_ABC& converter );
             LocalWeather( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocalWeather();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const;
    void Serialize( xml::xostream& xos ) const;
    void SetPosition( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight );
    void SetPeriod( const QDateTime& start, const QDateTime& end );
    geometry::Point2f GetTopLeft() const;
    geometry::Point2f GetBottomRight() const;
    QDateTime GetStartTime() const;
    QDateTime GetEndTime() const;
    bool CheckValidity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LocalWeather& operator=( const LocalWeather& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@
    const kernel::CoordinateConverter_ABC& converter_;
    unsigned long id_;
    QString name_;
    geometry::Point2f topLeft_;
    geometry::Point2f bottomRight_;
    QDateTime startTime_;
    QDateTime endTime_;
    //@}
};

#endif // __LocalWeather_h_
