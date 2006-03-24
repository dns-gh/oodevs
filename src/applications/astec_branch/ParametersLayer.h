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

#include "graphics/MapLayer_ABC.h"
class GlTools_ABC;
class ShapeHandler_ABC;

// =============================================================================
/** @class  ParametersLayer
    @brief  ParametersLayer // $$$$ AGE 2006-03-24: points, lines, polygons
*/
// Created: AGE 2006-03-23
// =============================================================================
class ParametersLayer : public MapLayer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ParametersLayer( const GlTools_ABC& tools );
    virtual ~ParametersLayer();
    //@}

    //! @name Operations
    //@{
    void Start( ShapeHandler_ABC& handler, int type = GL_LINE_STRIP, unsigned expected = std::numeric_limits< unsigned >::max() );

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

    bool IsDone() const;
    void NotifyDone();
    //@}

private:
    //! @name Member data
    //@{
    const GlTools_ABC& tools_;
    ShapeHandler_ABC* handler_;
    geometry::Rectangle2f world_;
    int type_;
    unsigned expected_;
    T_PointVector points_;
    //@}
};

#endif // __ParametersLayer_h_
