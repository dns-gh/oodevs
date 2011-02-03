// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Weather_h_
#define __Weather_h_

#include "clients_kernel/Types.h"

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  Weather
    @brief  Weather
*/
// Created: SBO 2006-12-19
// =============================================================================
class Weather
{
public:
    //! @name Constructors/Destructor
    //@{
             Weather();
    explicit Weather( xml::xistream& xis );
    virtual ~Weather();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Weather& operator=( const Weather& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    int windSpeed_;
    int windDirection_;
    int cloudFloor_;
    int cloudCeiling_;
    int cloudDensity_;
    E_WeatherType type_;
    //@}
};

#endif // __Weather_h_
