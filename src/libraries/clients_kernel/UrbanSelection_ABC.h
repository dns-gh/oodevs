// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UrbanSelection_ABC_h_
#define __UrbanSelection_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
// =============================================================================
/** @class  UrbanSelection_ABC
    @brief  Urban selection declaration
*/
// Created: LGY 2011-04-19
// =============================================================================
class UrbanSelection_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanSelection_ABC() {}
    virtual ~UrbanSelection_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ToggleSelection() = 0;
    //@}
};

}

#endif // __UrbanSelection_ABC_h_
