// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityHelpers_h_
#define __EntityHelpers_h_

namespace kernel
{
    class Entity_ABC;

// =============================================================================
/** @class  EntityHelpers
    @brief  EntityHelpers
*/
// Created: MMC 2012-10-03
// =============================================================================
class EntityHelpers
{
public:
    //! @name Helpers
    //@{
    static bool IsLogisticBase( const Entity_ABC& entity );
    //@}
};

}

#endif // __EntityHelpers_h_
