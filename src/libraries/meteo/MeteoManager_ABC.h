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

#include "PHY_Meteo.h"
#include "PHY_Lighting.h"


// =============================================================================
/** @class  MeteoManager_ABC
    @brief  MeteoManager_ABC
*/
// Created: HBD 2010-03-10
// =============================================================================
class MeteoManager_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
     MeteoManager_ABC() {};
     virtual ~MeteoManager_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void                 RegisterMeteo  ( PHY_Meteo& ) = 0;
    virtual void                 UnregisterMeteo( PHY_Meteo& ) = 0;
    virtual const PHY_Lighting&  GetLighting     () const = 0;
    //@}
};

#endif // __MeteoManager_ABC_h_
