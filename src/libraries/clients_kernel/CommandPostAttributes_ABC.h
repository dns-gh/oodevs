// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __CommandPostAttributes_ABC_h_
#define __CommandPostAttributes_ABC_h_

#include "Extension_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
// =============================================================================
/** @class  CommandPostAttributes_ABC
    @brief  Command post attributes declaration
*/
// Created: LGY 2012-02-06
// =============================================================================
class CommandPostAttributes_ABC : public Extension_ABC
                                , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CommandPostAttributes_ABC() {}
    virtual ~CommandPostAttributes_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsCommandPost() const = 0;
    //@}
};

}

#endif // __CommandPostAttributes_ABC_h_
