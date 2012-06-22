// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __clientskernel_Color_h_
#define __clientskernel_Color_h_

#include "Color_ABC.h"
#include <boost/optional.hpp>

namespace kernel
{
// =============================================================================
/** @class  Color
    @brief  Color
*/
// Created: LDC 2012-05-04
// =============================================================================
class Color : public Color_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Color();
    virtual ~Color();
    //@}

    //! @name Operations
    //@{
    virtual bool IsOverriden() const;
    virtual const T_Color& GetColor() const;
    virtual void ChangeColor( const T_Color& color );
    virtual void Clear();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Color( const Color& );            //!< Copy constructor
    Color& operator=( const Color& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    boost::optional< T_Color > color_;
    //@}
};
}

#endif // __clientskernel_Color_h_