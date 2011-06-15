// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UrbanColor_h_
#define __UrbanColor_h_

#include "clients_kernel/UrbanColor_ABC.h"

namespace urban
{
    class ColorAttribute;
}

// =============================================================================
/** @class  UrbanColor
    @brief  UrbanColor
*/
// Created: LGY 2011-04-19
// =============================================================================
class UrbanColor : public kernel::UrbanColor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanColor( const urban::ColorAttribute& colorAttribute );
    virtual ~UrbanColor();
    //@}

    //! @name Operations
    //@{
    virtual unsigned short Red() const;
    virtual unsigned short Green() const;
    virtual unsigned short Blue() const;
    virtual float Alpha() const;
    virtual void SetColor( unsigned short red, unsigned short green, unsigned short blue );
    virtual void Restore();
    //@}

private:
    //! @name Types
    //@{
    struct Color
    {
        Color( unsigned short red, unsigned short green, unsigned short blue )
            : red_  ( red )
            , green_( green )
            , blue_ ( blue )
        {}
        unsigned short red_;
        unsigned short green_;
        unsigned short blue_;
    };
    //@}

private:
    //! @name Member data
    //@{
    Color initial_;
    Color current_;
    float alpha_;
    //@}
};

#endif // __UrbanColor_h_
