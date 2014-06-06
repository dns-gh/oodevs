// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherModel_h_
#define __WeatherModel_h_

#include "clients_kernel/Types.h"
#include <tools/Resolver.h>
#include <QtCore/qdatetime.h>

namespace kernel
{
    class Controller;
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class Loader_ABC;
    class SchemaWriter_ABC;
}

namespace weather
{
    class Meteo;
    class MeteoLocal;
}

// =============================================================================
/** @class  WeatherModel
@brief  WeatherModel
*/
// Created: SBO 2006-12-19
// =============================================================================
class WeatherModel : public tools::Resolver< weather::MeteoLocal >
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherModel( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter );
    virtual ~WeatherModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Load( const tools::Loader_ABC& fileLoader, const tools::Path& filename );
    void Serialize( const tools::Path& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    const QDateTime& GetDate() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeatherModel( const WeatherModel& );            //!< Copy constructor
    WeatherModel& operator=( const WeatherModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Read             ( xml::xistream& xis );
    void ReadExerciseDate ( xml::xistream& xis );
    void ReadEphemerides  ( xml::xistream& xis );
    void ReadGlobalWeather( xml::xistream& xis );
    void ReadLocalWeather ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    //@}

public:
    //! @name Member data
    QDateTime                       time_;
    QTime                           sunrise_;
    QTime                           sunset_;
    E_LightingType                  dayLighting_;
    E_LightingType                  nightLighting_;
    std::unique_ptr< weather::Meteo > globalWeather_;
    //@}
};

#endif // __WeatherModel_h_
