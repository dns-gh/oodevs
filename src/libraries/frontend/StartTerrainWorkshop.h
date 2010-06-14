// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StartTerrainWorkshop_h_
#define __StartTerrainWorkshop_h_

#include "SpawnCommand.h"

namespace frontend
{

// =============================================================================
/** @class  StartTerrainWorkshop
    @brief  StartTerrainWorkshop
*/
// Created: JSR 2010-06-11
// =============================================================================
class StartTerrainWorkshop : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
             StartTerrainWorkshop( const tools::GeneralConfig& config, bool attach = false );
    virtual ~StartTerrainWorkshop();
    //@}

    //! @name Operations
    //@{
    static bool IsAvailable();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StartTerrainWorkshop( const StartTerrainWorkshop& );            //!< Copy constructor
    StartTerrainWorkshop& operator=( const StartTerrainWorkshop& ); //!< Assignment operator
    //@}
};

}

#endif // __StartTerrainWorkshop_h_
