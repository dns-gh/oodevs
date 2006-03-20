// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlWidget_h_
#define __GlWidget_h_

#include "graphics/MapWidget.h"

#include "WorldParameters.h"
#include "GlTools_ABC.h"
#include "SetGlOptions.h"

class CoordinateConverter;
class Controller;
class ActionController;
class ColorStrategy;
class SelectionLayer_ABC;

// =============================================================================
/** @class  GlWidget
    @brief  GlWidget
*/
// Created: AGE 2006-03-15
// =============================================================================
class GlWidget : private WorldParameters, private SetGlOptions, public MapWidget, public GlTools_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GlWidget( QWidget* pParent, const std::string& scipioXml,
                       Controller& controller, ActionController& actions, const CoordinateConverter& converter );
    virtual ~GlWidget();
    //@}

    //! @name Operations
    //@{
    virtual float Pixels() const;
    virtual unsigned short StipplePattern() const;

    virtual void  DrawCross      ( const geometry::Point2f& at, float size = -1.f ) const;
    virtual void  DrawLine       ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void  DrawLines      ( const T_PointVector& points ) const;
    virtual void  DrawArrow      ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f ) const;
    virtual void  DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.3f, float size = -1.f ) const;
    virtual void  DrawCircle     ( const geometry::Point2f& center, float radius = -1.f ) const;
    virtual void  DrawDisc       ( const geometry::Point2f& center, float radius = -1.f ) const;
    virtual void  DrawRectangle  ( const geometry::Rectangle2f& rect ) const;
    virtual void  Print          ( const std::string& message, const geometry::Point2f& where ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlWidget( const GlWidget& );            //!< Copy constructor
    GlWidget& operator=( const GlWidget& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    unsigned int GenerateCircle();
    //@}

private:
    //! @name Member data
    //@{
    ColorStrategy& strategy_;

    int windowHeight_;
    int windowWidth_;
    geometry::Rectangle2f viewport_;
    unsigned int frame_;
    unsigned int circle_;
    //@}
};

#endif // __GlWidget_h_
