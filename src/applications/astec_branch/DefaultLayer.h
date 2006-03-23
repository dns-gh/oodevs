// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DefaultLayer_h_
#define __DefaultLayer_h_

#include "graphics/MapLayer_ABC.h"
class Controllers;

// =============================================================================
/** @class  DefaultLayer
    @brief  DefaultLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class DefaultLayer : public MapLayer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DefaultLayer( Controllers& controllers );
    virtual ~DefaultLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DefaultLayer( const DefaultLayer& );            //!< Copy constructor
    DefaultLayer& operator=( const DefaultLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    geometry::Point2f point_;
    //@}
};

#endif // __DefaultLayer_h_
