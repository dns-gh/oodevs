// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Color_ABC_h_
#define __Color_ABC_h_

#include "Extension_ABC.h"
#include "Serializable_ABC.h"
#include <boost/noncopyable.hpp>
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
    //! @name Constructors/Destructor
    //@{
             Color_ABC() {}
    virtual ~Color_ABC() {}
    //@}

    //! @name Types
    //@{
    typedef boost::tuple< unsigned int ,unsigned int, unsigned int > T_Color;
    //@}

    //! @name Operations
    //@{
    virtual bool IsOverride() const = 0;
    virtual const T_Color& GetColor() const = 0;
    virtual void ChangeColor( const T_Color& color ) = 0;
    virtual void Clear() = 0;
    //@}
};

}

#endif // __Color_ABC_h_
