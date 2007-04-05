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
#include "clients_kernel/WorldParameters.h"
#include "SetGlOptions.h"
#include "View_ABC.h"
#include "GlToolsBase.h"

namespace kernel
{
    class Controllers;
    class ExerciseConfig;
}

namespace gui
{  
    class MiniView;
    class IconLayout;
    class IconHandler_ABC;

// =============================================================================
/** @class  GlWidget
    @brief  GlWidget
*/
// Created: AGE 2006-03-15
// =============================================================================
class GlWidget : private kernel::WorldParameters
               , private SetGlOptions
               , public MapWidget
               , public GlToolsBase
               , public View_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GlWidget( QWidget* pParent, kernel::Controllers& controllers, const kernel::ExerciseConfig& config, IconLayout& iconLayout );
    virtual ~GlWidget();
    //@}

    //! @name Operations
    //@{
    void AddMiniView   ( MiniView* view );
    void RemoveMiniView( MiniView* view );

    void CreateIcon( const std::string& symbol, const QColor& color, IconHandler_ABC& handler );
    void CreateIcon( const std::string& symbol, const std::string& level, const QColor& color, IconHandler_ABC& handler );
    //@}

    //! @name Operations
    //@{
    virtual unsigned short StipplePattern( int factor = 1 ) const;
    virtual float Pixels( const geometry::Point2f& at = geometry::Point2f() ) const;

    virtual void DrawCross      ( const geometry::Point2f& at, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLine       ( const geometry::Point2f& from, const geometry::Point2f& to ) const;
    virtual void DrawLines      ( const T_PointVector& points ) const;
    virtual void DrawArrow      ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawArc        ( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2 ) const;
    virtual void DrawCircle     ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawDisc       ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const;
    virtual void DrawLife       ( const geometry::Point2f& center, float height, float factor = 1.f ) const;
    virtual void Print          ( const std::string& message, const geometry::Point2f& where ) const;
    virtual void DrawApp6Symbol ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f ) const;
    virtual void DrawIcon       ( const char** xpm, const geometry::Point2f& where, float size = -1.f, E_Unit unit = meters ) const;
    virtual void DrawImage      ( const QImage& image, const geometry::Point2f& where ) const;
    virtual void DrawCell       ( const geometry::Point2f& center ) const;
    virtual void DrawFlag       ( const geometry::Point2f& center ) const;

    virtual void CenterOn( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlWidget( const GlWidget& );            //!< Copy constructor
    GlWidget& operator=( const GlWidget& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< MiniView* >   T_Views;
    typedef T_Views::iterator         IT_Views;
    typedef T_Views::const_iterator  CIT_Views;

    struct T_IconTask
    {
        T_IconTask( const std::string& name, const QColor& color, IconHandler_ABC& handler )
            : name( name ), color( color ), handler( &handler ) {};
        T_IconTask( const std::string& name, const std::string& name2, const QColor& color, IconHandler_ABC& handler )
            : name( name ), name2( name2 ), color( color ), handler( &handler ) {};
        std::string name;
        std::string name2;
        QColor color;
        IconHandler_ABC* handler;
    };
    typedef std::vector< T_IconTask >                    T_IconTasks;
    typedef T_IconTasks::const_iterator                CIT_IconTasks;
    //@}

    //! @name Helpers
    //@{
    virtual void paintGL();
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void updateGL();
    unsigned int GenerateCircle();

    void RenderMiniViews();
    void RenderMiniView( MiniView& view );
    void RenderIcons();
    void RenderIcon( const T_IconTask& task, const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Member data
    //@{
    static const unsigned miniViewSide_ = 128;
    static const unsigned iconSide_     = 128;
 
    int windowHeight_;
    int windowWidth_;
    unsigned int circle_;
 
    geometry::Rectangle2f viewport_;
    unsigned int frame_;

    T_Views views_;
    IconLayout& iconLayout_;

    T_IconTasks tasks_;
 
    int listBase_;
    //@}
};

}

#endif // __GlWidget_h_
