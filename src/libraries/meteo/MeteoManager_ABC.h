// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MeteoManager_h_
#define __MeteoManager_h_

#include <boost/shared_ptr.hpp>

namespace weather
{
    class PHY_Lighting;
    class PHY_Meteo;

// =============================================================================
/** @class  MeteoManager_ABC
    @brief  Meteo manager declaration
*/
// Created: HBD 2010-03-10
// =============================================================================
class MeteoManager_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MeteoManager_ABC() {}
    virtual ~MeteoManager_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void RegisterMeteo( boost::shared_ptr< weather::PHY_Meteo > element ) = 0;
    virtual void UnregisterMeteo( boost::shared_ptr< weather::PHY_Meteo > element ) = 0;
    virtual const PHY_Lighting& GetLighting() const = 0;
    //@}
};

}

#endif // __MeteoManager_ABC_h_
