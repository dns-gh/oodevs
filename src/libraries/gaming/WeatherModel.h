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

#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class StartFireEffect;
    class StopFireEffect;
}

namespace kernel
{
    class Controller;
}

class AmmoEffect;
class Model;

// =============================================================================
/** @class  WeatherModel
    @brief  WeatherModel
*/
// Created: AGE 2006-04-04
// =============================================================================
class WeatherModel : public tools::Resolver< AmmoEffect >
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherModel( kernel::Controller& controller, Model& model );
    virtual ~WeatherModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    void CreateAmmoEffect( const sword::StartFireEffect& message );
    void DeleteAmmoEffect( const sword::StopFireEffect& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Model& model_;
    //@}
};

#endif // __WeatherModel_h_
