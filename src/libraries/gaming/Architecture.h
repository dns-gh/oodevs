// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Architecture_h_
#define __Architecture_h_

#include "clients_gui/Architecture.h"

namespace sword
{
    class UrbanAttributes;
}

// =============================================================================
/** @class  Architecture
    @brief  Architecture
*/
// Created: LGY 2011-04-15
// =============================================================================
class Architecture : public gui::Architecture
{
public:
    //! @name Constructors/Destructor
    //@{
             Architecture( const sword::UrbanAttributes& message, kernel::PropertiesDictionary& dictionary );
    virtual ~Architecture();
    //@}
};

#endif // __Architecture_h_
