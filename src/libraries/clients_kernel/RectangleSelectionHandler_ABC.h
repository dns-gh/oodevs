// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RectangleSelectionHandler_ABC_h_
#define __RectangleSelectionHandler_ABC_h_

#include <geometry/Types.h>

namespace kernel
{
// =============================================================================
/** @class  RectangleSelectionHandler_ABC
    @brief  RectangleSelectionHandler_ABC
*/
// Created: JSR 2012-05-23
// =============================================================================
class RectangleSelectionHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RectangleSelectionHandler_ABC() {}
    virtual ~RectangleSelectionHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void HandleRectangleSelection( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight ) = 0;
    //@}
};

}

#endif // __RectangleSelectionHandler_ABC_h_
