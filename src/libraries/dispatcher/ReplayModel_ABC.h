// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReplayModel_ABC_h_
#define __ReplayModel_ABC_h_

#include <boost/noncopyable.hpp>

namespace dispatcher
{
// =============================================================================
/** @class  ReplayModel_ABC
    @brief  Replay model definition
*/
// Created: AGE 2007-07-05
// =============================================================================
class ReplayModel_ABC : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ReplayModel_ABC() {}
    virtual ~ReplayModel_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void StartSynchronisation() = 0;
    virtual void EndSynchronisation() = 0;
    virtual bool IsSynching() const = 0;
    //@}
};

}

#endif // __ReplayModel_ABC_h_
