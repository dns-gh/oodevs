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
             LocalWeather();
    explicit LocalWeather( xml::xistream& xis );
    virtual ~LocalWeather();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    QString GetName() const;
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LocalWeather& operator=( const LocalWeather& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@
    unsigned long id_;
    QString name_;
    std::string topLeft_;
    std::string bottomRight_;
    //@}
};

#endif // __LocalWeather_h_
