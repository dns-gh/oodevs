// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GLWidget_h_
#define __GLWidget_h_

#include "svgl/Point.h"
#include "svgl/BoundingBox.h"
#include "svgl/TextRenderer.h"
#pragma warning(push, 0)
#include <QtOpenGL/qgl.h>
#pragma warning(pop)
#include <string>

namespace svg {
    class Node_ABC;
    class References;
    class Property_ABC;
    class RenderingContext;
}

// =============================================================================
/** @class  GLWidget
    @brief  GLWidget
*/
// Created: AGE 2006-08-03
// =============================================================================
class GLWidget : public QGLWidget
{

public:
    //! @name Constructors/Destructor
    //@{
             GLWidget( QWidget* parent, const std::string& filename );
    virtual ~GLWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GLWidget( const GLWidget& );            //!< Copy constructor
    GLWidget& operator=( const GLWidget& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();

    virtual void mouseReleaseEvent( QMouseEvent* );
    virtual void wheelEvent       ( QWheelEvent* );
    virtual void keyPressEvent    ( QKeyEvent* );  

    void ComputeViewport();
    geometry::Point RetrieveCoordinates( const QMouseEvent& e );
    void Parse( const std::string& filename );
    //@}

private:
    //! @name Member data
    //@{
    svg::TextRenderer renderer_;
    geometry::Point center_;
    geometry::Point topLeft_, bottomRight_;
    float zoom_;
    float ratio_;
    svg::RenderingContext* context_;
    svg::References* references_;
    svg::Node_ABC* root_;
    svg::Property_ABC* color_;
    bool compile_;
    std::string filename_;
    //@}
};

#endif // __GLWidget_h_
