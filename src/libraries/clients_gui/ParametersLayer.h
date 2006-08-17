// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParametersLayer_h_
#define __ParametersLayer_h_

#include "Layer_ABC.h"
class GlTools_ABC;
class ShapeHandler_ABC;
class Location_ABC;
class Positions;

// =============================================================================
/** @class  ParametersLayer
    @brief  ParametersLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class ParametersLayer : public Layer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ParametersLayer( const GlTools_ABC& tools );
    virtual ~ParametersLayer();
    //@}

    //! @name Operations
    //@{
    void StartPoint  ( ShapeHandler_ABC& handler );
    void StartLine   ( ShapeHandler_ABC& handler );
    void StartPolygon( ShapeHandler_ABC& handler );
    void StartCircle ( ShapeHandler_ABC& handler );
    void StartPath   ( ShapeHandler_ABC& handler, const Positions& position );
    void Start       ( ShapeHandler_ABC& handler, Location_ABC& location );

    void AddPoint( const geometry::Point2f& point );

    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParametersLayer( const ParametersLayer& );            //!< Copy constructor
    ParametersLayer& operator=( const ParametersLayer& ); //!< Assignement operator
    //@}
   
    //! @name Helpers
    //@{
    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );

    void NotifyDone();
    //@}

private:
    //! @name Member data
    //@{
    const GlTools_ABC& tools_;
    ShapeHandler_ABC* handler_;
    Location_ABC* current_;
    geometry::Rectangle2f world_;
    //@}
};

#endif // __ParametersLayer_h_
