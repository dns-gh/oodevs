// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ElevationResolver_h_
#define __ElevationResolver_h_

#include "ElevationResolver_ABC.h"

namespace kernel
{
    class DetectionMap;
}

namespace gui
{
// =============================================================================
/** @class  ElevationResolver
    @brief  Elevation resolver
*/
// Created: LGY 2010-09-27
// =============================================================================
class ElevationResolver : public ElevationResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ElevationResolver( const kernel::DetectionMap& elevation );
    virtual ~ElevationResolver();
    //@}

    //! @name Operations
    //@{
    virtual float Compute( unsigned int pourcentage ) const;
    //@}

private:
    //! @name Member data
    //@{
     const kernel::DetectionMap& elevation_;
    //@}
};
}

#endif // __ElevationResolver_h_
