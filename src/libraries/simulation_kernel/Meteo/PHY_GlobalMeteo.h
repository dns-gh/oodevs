// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_GlobalMeteo_h_
#define __PHY_GlobalMeteo_h_

#include "meteo/Meteo.h"

namespace xml
{
    class xistream;
}

namespace weather
{
    class PHY_Lighting;
}

// =============================================================================
/** @class  PHY_GlobalMeteo
    @brief  PHY_GlobalMeteo
*/
// Created: HBD 2010-03-25
// =============================================================================
class PHY_GlobalMeteo : public weather::Meteo
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_GlobalMeteo( xml::xistream& xis, const weather::PHY_Lighting& light, unsigned int timeStep );
    virtual ~PHY_GlobalMeteo();
    //@}

    //! @name Operations
    //@{
    virtual void SendCreation() const;
    //@}
};

#endif // __PHY_GlobalMeteo_h_
