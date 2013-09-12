// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StartAuthoring_h_
#define __StartAuthoring_h_

#include "SpawnCommand.h"

namespace frontend
{

// =============================================================================
/** @class  StartAuthoring
    @brief  StartAuthoring
*/
// Created: JSR 2010-06-10
// =============================================================================
class StartAuthoring : public SpawnCommand
{
public:
    //! @name Constructors/Destructor
    //@{
             StartAuthoring( const tools::GeneralConfig& config );
    virtual ~StartAuthoring();
    //@}
};

}

#endif // __StartAuthoring_h_
