// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __clientsgui_Color_h_
#define __clientsgui_Color_h_

#include "clients_kernel/Color_ABC.h"
#include <boost/optional.hpp>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Entity_ABC;
}

namespace sword
{
    class RgbColor;
}

namespace gui
{
// =============================================================================
/** @class  Color
    @brief  Color
*/
// Created: LDC 2012-05-04
// =============================================================================
class Color : public kernel::Color_ABC
{
public:
             Color();
    explicit Color( const sword::RgbColor& color );
    explicit Color( const kernel::Entity_ABC& parent );
    explicit Color( xml::xistream& xis );
    virtual ~Color();

    virtual bool IsOverriden() const;
    virtual const T_Color& GetColor() const;
    virtual void ChangeColor( const T_Color& color );
    virtual void ChangeColor( xml::xistream& xis );
    virtual void Clear();
    virtual const boost::optional< T_Color >& GetBaseColor() const;

    virtual void SerializeAttributes( xml::xostream& xos ) const;

private:
    boost::optional< T_Color > color_;
    const boost::optional< T_Color > base_;
};
}

#endif // __clientsgui_Color_h_
