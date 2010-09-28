// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ElevationResolver_ABC_h_
#define __ElevationResolver_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{
// =============================================================================
/** @class  ElevationResolver_ABC
    @brief  Elevation resolver declaration
*/
// Created: LGY 2010-09-27
// =============================================================================
class ElevationResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ElevationResolver_ABC() {}
    virtual ~ElevationResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual float Compute( unsigned int pourcentage ) const = 0;
    //@}
};

}

#endif // __ElevationResolver_ABC_h_
