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

#include "clients_kernel/Color_ABC.h"

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
class Color : public kernel::Color_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Color( const sword::RgbColor& color );
    virtual ~Color();
    //@}

    //! @name Operations
    //@{
    virtual bool IsOverride() const;
    virtual const T_Color& GetColor() const;
    virtual void ChangeColor( const T_Color& color );
    virtual void Clear();
    //@}

private:
    //! @name Member data
    //@{
    T_Color color_;
    //@}
};

#endif // __Color_h_
