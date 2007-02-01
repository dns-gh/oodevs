// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainFactory_ABC_h_
#define __TerrainFactory_ABC_h_

#include "Messages.h"

namespace frontend
{
    class Terrain;

// =============================================================================
/** @class  TerrainFactory_ABC
    @brief  TerrainFactory_ABC
*/
// Created: SBO 2007-01-29
// =============================================================================
class TerrainFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TerrainFactory_ABC() {}
    virtual ~TerrainFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Terrain* Create( const ASN1T_MsgTerrainCreation& message ) = 0;
    //@}
};

}

#endif // __TerrainFactory_ABC_h_
