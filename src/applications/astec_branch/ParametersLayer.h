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
    void StartPoints( ShapeHandler_ABC& handler, unsigned expected = 1 );
    void StartLine( ShapeHandler_ABC& handler, unsigned expected = std::numeric_limits< unsigned >::max() );
    void StartPolygon( ShapeHandler_ABC& handler, unsigned expected = std::numeric_limits< unsigned >::max() );
    void StartCircle( ShapeHandler_ABC& handler );

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
        
    //! @name Types
    //@{
    enum E_Type {
        points,
        lines,
        polygon,
        circle
    };
    //@}

    //! @name Helpers
    //@{
    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );

    bool IsDone() const;
    void NotifyDone();

    void Start( ShapeHandler_ABC& handler, unsigned expected, E_Type type );
    //@}

private:
    //! @name Member data
    //@{
    const GlTools_ABC& tools_;
    ShapeHandler_ABC* handler_;
    geometry::Rectangle2f world_;
    E_Type type_;
    unsigned expected_;
    T_PointVector points_;
    //@}
};

#endif // __ParametersLayer_h_
