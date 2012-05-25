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
    virtual unsigned short Red() const;
    virtual unsigned short Green() const;
    virtual unsigned short Blue() const;
    virtual float Alpha() const;
    virtual void SetColor( unsigned short red, unsigned short green, unsigned short blue );
    virtual void Restore();
    //@}

protected:
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

protected:
    //! @name Member data
    //@{
    Color initial_;
    Color current_;
    float alpha_;
    //@}
};

}

#endif // __UrbanColor_ABC_h_
