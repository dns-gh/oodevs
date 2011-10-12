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

#include "tools/WorldParameters.h"
#include "SetGlOptions.h"
#include "View_ABC.h"
#include "GlToolsBase.h"
#include "MapWidget_ABC.h"
#include <graphics/MapWidget.h>

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class Controllers;
    class UrbanColor_ABC;
}

namespace gui
{
    class IconLayout;
    class GlRenderPass_ABC;
    class TextureRenderPass;

// =============================================================================
/** @class  GlWidget
    @brief  GlWidget
*/
// Created: AGE 2006-03-15
// =============================================================================
class GlWidget : private tools::WorldParameters
               , private SetGlOptions
               , public MapWidget
               , public MapWidget_ABC
               , public GlToolsBase
               , public View_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GlWidget( QWidget* pParent, kernel::Controllers& controllers, const tools::ExerciseConfig& config, IconLayout& iconLayout );
    virtual ~GlWidget();
    //@}

    //! @name Operations
    //@{
    void SetPassOrder( const std::string& names );
    void AddPass( GlRenderPass_ABC& pass );
    void RemovePass( GlRenderPass_ABC& pass );

    std::string GetCurrentPass() const;
    virtual void PaintLayers();
    virtual unsigned int Width() const;
    virtual unsigned int Height() const;
    virtual geometry::Rectangle2f Viewport() const;
    //@}

    //! @name Operations
    //@{
    virtual unsigned short StipplePattern( int factor = 1 ) const;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;
    virtual float Zoom() const;
    virtual float GetAdaptiveZoomFactor() const;

    virtual void DrawCross        ( const geometry::Point2f& at, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLine         ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines        ( const T_PointVector& points ) const;
    virtual void DrawRectangle    ( const T_PointVector& points ) const;
    virtual void DrawPolygon      ( const T_PointVector& points ) const;
    virtual void DrawDecoratedPolygon( const geometry::Polygon2f& polygon, const kernel::UrbanColor_ABC& urbanColor, const std::string& name, unsigned int height, bool selected ) const;
    virtual void DrawArrow        ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawCurvedArrow  ( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawArc          ( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2 ) const;
    virtual void DrawCircle       ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawDisc         ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLife         ( const geometry::Point2f& center, float height, float factor = 1.f ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where, const QFont& font ) const;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f ) const;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f ) const;
    virtual void DrawIcon         ( const char** xpm, const geometry::Point2f& where, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawImage        ( const QImage& image, const geometry::Point2f& where ) const;
    virtual void DrawCell         ( const geometry::Point2f& center ) const;
    virtual void DrawSvg          ( const std::string& svg, const geometry::Point2f& center, float ratio = 1.f ) const;
    virtual void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined ) const;

    virtual void CenterOn( const geometry::Point2f& point );
    virtual void Zoom( float width );
    virtual void SetCurrentCursor( const QCursor& cursor );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GlWidget( const GlWidget& );            //!< Copy constructor
    GlWidget& operator=( const GlWidget& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    struct passLess
    {
        explicit passLess( const std::string& names ) : names_( names ){}
        bool operator()( GlRenderPass_ABC* lhs, GlRenderPass_ABC* rhs ) const;
        std::string names_;
    };
    typedef std::multiset< GlRenderPass_ABC*, passLess > T_RenderPasses;

    struct sFontLess : public std::binary_function< QFont, QFont, bool >
    {
        bool operator()( const QFont& lhs, const QFont& rhs ) const
        {
            return lhs.toString() < rhs.toString();
        }
    };

    typedef std::map< QFont, int, sFontLess > T_Fonts;
    typedef T_Fonts::const_iterator         CIT_Fonts;
    //@}

    //! @name Helpers
    //@{
    virtual void paintGL();
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void updateGL();
    unsigned int GenerateCircle();
    void UpdateStipple() const;
    void DrawTextLabel( const std::string& message, const geometry::Point2f& where, int baseSize = 12);

    void RenderPass( GlRenderPass_ABC& pass );
    //@}

private:
    //! @name Member data
    //@{
    int windowHeight_;
    int windowWidth_;
    unsigned int circle_;
    int minVisuScale_;
    int maxVisuScale_;
    geometry::Rectangle2f viewport_;
    unsigned int frame_;
    IconLayout& iconLayout_;
    T_RenderPasses passes_;
    std::string currentPass_;
    QFont currentFont_;
    //@}
};

}

#endif // __GlWidget_h_
