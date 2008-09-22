// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CreateTerrain_h_
#define __CreateTerrain_h_

#include "SpawnCommand.h"

namespace frontend
{

// =============================================================================
/** @class  CreateTerrain
    @brief  CreateTerrain
*/
// Created: AGE 2007-10-04
// =============================================================================
class CreateTerrain : public SpawnCommand
{

public:
    //! @name Constructors/Destructor
    //@{
             CreateTerrain( const tools::GeneralConfig& config, const QString& name, bool attach = false );
    virtual ~CreateTerrain();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreateTerrain( const CreateTerrain& );            //!< Copy constructor
    CreateTerrain& operator=( const CreateTerrain& ); //!< Assignment operator
    //@}
};

}

#endif // __CreateTerrain_h_
