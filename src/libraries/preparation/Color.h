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
#include <boost/optional.hpp>

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
class Color : public kernel::Color_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Color();
    explicit Color( const kernel::Entity_ABC& parent );
    explicit Color( xml::xistream& xis );
    virtual ~Color();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;

    virtual bool IsOverride() const;
    virtual const T_Color& GetColor() const;
    virtual void ChangeColor( const T_Color& color );
    virtual void Clear();
    //@}

private:
    //! @name Member data
    //@{
    boost::optional< T_Color > color_;
    //@}
};

#endif // __Color_h_
