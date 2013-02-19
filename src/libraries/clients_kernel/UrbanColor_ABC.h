// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UrbanColor_ABC_h_
#define __UrbanColor_ABC_h_

#include "Extension_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{

// $$$$ ABR 2012-06-04: TODO: Merge UrbanColor, UrbanBlockColor, Color, Color_ABC, etc. and use the property panel to change the color.

//! @name Types
//@{
struct UrbanBlockColor
{
    UrbanBlockColor( int red, int green, int blue, int alpha )
        : red_  ( red )
        , green_( green )
        , blue_ ( blue )
        , alpha_( alpha )
    {}
    int red_;
    int green_;
    int blue_;
    int alpha_;
};
//@}

// =============================================================================
/** @class  UrbanColor_ABC
    @brief  Urban color declaration
*/
// Created: LGY 2011-04-19
// =============================================================================
class UrbanColor_ABC : public Extension_ABC
                     , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanColor_ABC();
    virtual ~UrbanColor_ABC();
    //@}

    //! @name Operations
    //@{
    virtual int Red() const;
    virtual int Green() const;
    virtual int Blue() const;
    virtual float Alpha() const;
    virtual void SetColor( int red, int green, int blue, int alpha );
    virtual void SetColor( const UrbanBlockColor& color );
    virtual UrbanBlockColor GetColor() const;
    virtual void Restore();
    //@}

protected:
    //! @name Member data
    //@{
    UrbanBlockColor initial_;
    UrbanBlockColor current_;
    //@}
};

}

#endif // __UrbanColor_ABC_h_
