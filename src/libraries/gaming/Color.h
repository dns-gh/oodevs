// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Color_h_
#define __Color_h_

#include "clients_gui/Color.h"

namespace sword
{
    class RgbColor;
}

// =============================================================================
/** @class  Color
    @brief  Color
*/
// Created: LGY 2011-06-27
// =============================================================================
class Color : public gui::Color
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Color( const sword::RgbColor& color );
    virtual ~Color();
    //@}
};

#endif // __Color_h_
