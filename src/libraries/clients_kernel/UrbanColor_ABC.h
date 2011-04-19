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
class UrbanColor_ABC : public Extension_ABC,
                       private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanColor_ABC() {}
    virtual ~UrbanColor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned short Red() const = 0;
    virtual unsigned short Green() const = 0;
    virtual unsigned short Blue() const = 0;
    virtual float Alpha() const = 0;
    virtual void SetColor( unsigned short red, unsigned short green, unsigned short blue ) = 0;
    virtual void Restore() = 0;
    //@}
};

}

#endif // __UrbanColor_ABC_h_
