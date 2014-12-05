// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __kernel_Color_ABC_h_
#define __kernel_Color_ABC_h_

#include "Extension_ABC.h"
#include "Serializable_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <boost/tuple/tuple.hpp>

namespace kernel
{
// =============================================================================
/** @class  Color_ABC
    @brief  Color declaration
*/
// Created: LGY 2011-06-24
// =============================================================================
 class Color_ABC : public Extension_ABC
                 , public Serializable_ABC
                 , private boost::noncopyable
{
public:
             Color_ABC() {}
    virtual ~Color_ABC() {}

    typedef boost::tuple< unsigned int, unsigned int, unsigned int > T_Color;

    virtual bool IsOverriden() const = 0;
    virtual const T_Color& GetColor() const = 0;
    virtual void ChangeColor( const T_Color& color ) = 0;
    virtual void ChangeColor( xml::xistream& xis ) = 0;
    virtual void Clear() = 0;
    virtual const boost::optional< T_Color >& GetBaseColor() const = 0;

    void ChangeColor( const QColor& color )
    {
        ChangeColor( T_Color( color.red(), color.green(), color.blue() ) );
    }
    operator QColor() const
    {
        return QColor( GetColor().get< 0 >(), GetColor().get< 1 >(), GetColor().get< 2 >() );
    }
};

}

#endif // __kernel_Color_ABC_h_
