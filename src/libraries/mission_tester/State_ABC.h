// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __State_ABC_h_
#define __State_ABC_h_

#include <boost/noncopyable.hpp>

namespace mission_tester
{
// =============================================================================
/** @class  State_ABC
    @brief  State_ABC
*/
// Created: PHC 2011-03-30
// =============================================================================
class State_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             State_ABC() {};
    virtual ~State_ABC() {};
    //@}
};
}

#endif // __State_ABC_h_
