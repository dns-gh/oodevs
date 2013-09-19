// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __View_ABC_h_
#define __View_ABC_h_

#include <geometry/Types.h>

namespace gui
{

// =============================================================================
/** @class  View_ABC
    @brief  View_ABC
*/
// Created: AGE 2006-03-28
// =============================================================================
class View_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             View_ABC() {};
    virtual ~View_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void CenterOn( const geometry::Point2f& point ) = 0;
    virtual void Zoom( float width ) = 0;
    virtual geometry::Point2f GetCenter() const = 0;
    //@}
};

}

#endif // __View_ABC_h_
