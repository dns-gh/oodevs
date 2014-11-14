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

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  Color
    @brief  Color
*/
// Created: LGY 2011-06-24
// =============================================================================
class Color : public gui::Color
{
public:
             Color();
    explicit Color( const kernel::Entity_ABC& parent );
    explicit Color( xml::xistream& xis );
    virtual ~Color();

    virtual void ChangeColor( xml::xistream& xis );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
};

#endif // __Color_h_
