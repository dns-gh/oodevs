// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainsModel_h_
#define __TerrainsModel_h_

#include "Messages.h"
#include "clients_kernel/Resolver.h"

namespace frontend
{
    class Terrain;
    class TerrainFactory_ABC;

// =============================================================================
/** @class  TerrainsModel
    @brief  TerrainsModel
*/
// Created: SBO 2007-01-29
// =============================================================================
class TerrainsModel : public kernel::StringResolver< Terrain >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainsModel( TerrainFactory_ABC& factory );
    virtual ~TerrainsModel();
    //@}

    //! @name Operations
    //@{
    void CreateTerrain( const ASN1T_MsgTerrainCreation& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainsModel( const TerrainsModel& );            //!< Copy constructor
    TerrainsModel& operator=( const TerrainsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    TerrainFactory_ABC& factory_;
    //@}
};

}

#endif // __TerrainsModel_h_
