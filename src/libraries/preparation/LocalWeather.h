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
    explicit LocalWeather( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocalWeather();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    QString GetName() const;
    void Serialize( xml::xostream& xos ) const;
    void SetPosition( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight );
    geometry::Point2f GetTopLeft() const;
    geometry::Point2f GetBottomRight() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LocalWeather& operator=( const LocalWeather& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@
    const kernel::CoordinateConverter_ABC& converter_;
    unsigned long id_;
    QString name_;
    geometry::Point2f topLeft_;
    geometry::Point2f bottomRight_;
    //@}
};

#endif // __LocalWeather_h_
