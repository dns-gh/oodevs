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
#include "SetGlOptions.h"
#include "View_ABC.h"
#include "GlToolsBase.h"

class Controllers;

// =============================================================================
/** @class  GlWidget
    @brief  GlWidget
*/
// Created: AGE 2006-03-15
// =============================================================================
class GlWidget : private WorldParameters, private SetGlOptions, public MapWidget, public GlToolsBase, public View_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GlWidget( QWidget* pParent, Controllers& controllers, const std::string& scipioXml );
    virtual ~GlWidget();
    //@}

    //! @name Operations
    //@{
    virtual float Pixels() const;
    virtual unsigned short StipplePattern( int factor = 1 ) const;

    virtual void DrawCross      ( const geometry::Point2f& at, float size = -1.f ) const;
    virtual void DrawLine       ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines      ( const T_PointVector& points ) const;
    virtual void DrawArrow      ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f ) const;
    virtual void DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f ) const;
    virtual void DrawCircle     ( const geometry::Point2f& center, float radius = -1.f ) const;
    virtual void DrawDisc       ( const geometry::Point2f& center, float radius = -1.f ) const;
    virtual void DrawRectangle  ( const geometry::Rectangle2f& rect ) const;
    virtual void Print          ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void DrawApp6Symbol ( const std::string& symbol, const geometry::Point2f& where ) const;
    virtual void DrawIcon       ( const char** xpm, const geometry::Point2f& where, float size = -1.f ) const;

    virtual void CenterOn( const geometry::Point2f& point );
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
    int windowHeight_;
    int windowWidth_;
    unsigned int circle_;
    
    geometry::Rectangle2f viewport_;
    unsigned int frame_;
    //@}
};

#endif // __GlWidget_h_
