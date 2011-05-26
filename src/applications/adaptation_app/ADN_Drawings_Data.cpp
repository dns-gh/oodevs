// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Drawings_Data.h"
#include "ADN_Project_Data.h"
#include "clients_gui/DrawingTemplate.h"
#include "clients_gui/GlTooltip.h"
#include "clients_gui/TooltipsLayer.h"
#include "clients_kernel/GlTools_ABC.h"
#include "svgl/Color.h"
#include "svgl/RenderingContext.h"
#include "svgl/TextRenderer.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <qbitmap.h>
#include <qgl.h>

namespace
{
    class GlTooltip_ABC;

    class GlToolsSymbols : public kernel::GlTools_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 GlToolsSymbols() : GlTools_ABC() {}
        virtual ~GlToolsSymbols() {}
        //@}

        //! @name Options
        //@{
        virtual std::pair< bool, bool > UnSelect() const { return std::pair< bool, bool >( false, false ); }
        virtual void Select( bool, bool ) const {}
        virtual bool ShouldDisplay( const std::string& ) const { return true; }
        virtual bool ShouldDisplay( const std::string&, bool ) const { return true; }
        //@}

        //! @name Accessors
        //@{
        virtual float           Pixels( const geometry::Point2f& ) const { return 0.001f; } // $$$$ ABR 2011-04-21: hard coded
        virtual unsigned short  StipplePattern( int ) const { return 0; }
        virtual float           Zoom() const { return 0.f; }
        virtual float           GetAdaptiveZoomFactor() const { return 0.f; }
        //@}

        //! @name Operations
        //@{
        virtual std::auto_ptr< kernel::GlTooltip_ABC > CreateTooltip() const { return std::auto_ptr< kernel::GlTooltip_ABC >( 0 ); }
        virtual void SetCurrentColor  ( float, float, float, float ) {}
        virtual void SetCurrentCursor ( const QCursor& ) {}
        virtual void DrawCross        ( const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawLine         ( const geometry::Point2f&, const geometry::Point2f& ) const {}
        virtual void DrawLines        ( const T_PointVector& ) const {}
        virtual void DrawRectangle    ( const T_PointVector& ) const {}
        virtual void DrawPolygon      ( const T_PointVector& ) const {}
        virtual void DrawDecoratedPolygon( const geometry::Polygon2f&, const kernel::UrbanColor_ABC&, const std::string&, unsigned int, bool ) const {}
        virtual void DrawArrow        ( const geometry::Point2f&, const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawCurvedArrow  ( const geometry::Point2f&, const geometry::Point2f&, float, float, E_Unit ) const {}
        virtual void DrawArc          ( const geometry::Point2f&, const geometry::Point2f&, const geometry::Point2f& ) const {}
        virtual void DrawCircle       ( const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawDisc         ( const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawLife         ( const geometry::Point2f&, float, float ) const {}
        virtual void Print            ( const std::string&, const geometry::Point2f& ) const {}
        virtual void Print            ( const std::string&, const geometry::Point2f&, const QFont& ) const {}
        virtual void DrawApp6Symbol   ( const std::string&, const geometry::Point2f&, float, float ) const {}
        virtual void DrawApp6Symbol   ( const std::string&, const std::string&, const geometry::Point2f&, float, float ) const {}
        virtual void DrawIcon         ( const char**, const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawImage        ( const QImage&, const geometry::Point2f& ) const {}
        virtual void DrawCell         ( const geometry::Point2f& ) const {}
        virtual void DrawSvg          ( const std::string&, const geometry::Point2f&, float ) const {}
        virtual void DrawTacticalGraphics( const std::string&, const kernel::Location_ABC&, bool ) const {}
        //@}
    };
}


// -----------------------------------------------------------------------------
// ADN_Drawings_Data::SymbolInfo
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::SymbolInfo
// Created: ABR 2011-04-18
// -----------------------------------------------------------------------------
ADN_Drawings_Data::DrawingInfo::DrawingInfo( xml::xistream& xis, svg::TextRenderer& renderer, kernel::GlTools_ABC& tools )
    : template_( new gui::DrawingTemplate( xis, "Tactical graphics", renderer ) ) // $$$$ ABR 2011-04-18: hard coded
    , tools_   ( tools )
    , strName_ ( template_->GetName().ascii() )
{
    Initialize();
    Draw();
    glFlush();
    QImage image( SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE, 32 );
    glReadPixels( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE, GL_BGRA_EXT, GL_UNSIGNED_BYTE, image.bits() );
    glFlush();
    pixmap_ = new QPixmap( image.mirror().smoothScale( QSize( SYMBOL_PIXMAP_SIZE, SYMBOL_PIXMAP_SIZE ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::~SymbolInfo
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Drawings_Data::DrawingInfo::~DrawingInfo()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::GetNodeName
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
std::string ADN_Drawings_Data::DrawingInfo::GetNodeName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::GetItemName
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
std::string ADN_Drawings_Data::DrawingInfo::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::GetPixmap
// Created: ABR 2011-04-21
// -----------------------------------------------------------------------------
const QPixmap& ADN_Drawings_Data::DrawingInfo::GetPixmap() const
{
    return *pixmap_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::GetCode
// Created: ABR 2011-04-21
// -----------------------------------------------------------------------------
const std::string ADN_Drawings_Data::DrawingInfo::GetCode() const
{
    return template_->GetCode().ascii();
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetGeometry
// Created: ABR 2011-04-21
// -----------------------------------------------------------------------------
const std::string ADN_Drawings_Data::DrawingInfo::GetGeometry() const
{
    return template_->GetType().ascii();
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::Initialize
// Created: ABR 2011-04-20
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::DrawingInfo::Initialize()
{
    glShadeModel( GL_SMOOTH );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnableClientState( GL_VERTEX_ARRAY );
    glLineWidth( 1.f );
    glColor3f( 1.f, 1.f, 1.f );
    glDisable( GL_DEPTH_TEST );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::Draw
// Created: ABR 2011-04-20
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::DrawingInfo::Draw()
{
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glEnable( GL_LINE_SMOOTH );
    glPushMatrix();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glViewport( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
    glOrtho( 0.0f, SYMBOL_ICON_SIZE, 0.0f, SYMBOL_ICON_SIZE, 0, 1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // $$$$ ABR 2011-04-21: draw background
    glColor3f( 0.f, 0.f, 0.f );
    glBegin( GL_QUADS );
        glVertex2f(              0.f,              0.f );
        glVertex2f(              0.f, SYMBOL_ICON_SIZE );
        glVertex2f( SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
        glVertex2f( SYMBOL_ICON_SIZE,              0.f );
    glEnd();
    glColor3f( 0.9f, 0.9f, 0.9f );
    glBegin( GL_QUADS );
        glVertex2f(                    SYMBOL_BG_MARGIN,                    SYMBOL_BG_MARGIN );
        glVertex2f(                    SYMBOL_BG_MARGIN, SYMBOL_ICON_SIZE - SYMBOL_BG_MARGIN );
        glVertex2f( SYMBOL_ICON_SIZE - SYMBOL_BG_MARGIN, SYMBOL_ICON_SIZE - SYMBOL_BG_MARGIN );
        glVertex2f( SYMBOL_ICON_SIZE - SYMBOL_BG_MARGIN,                    SYMBOL_BG_MARGIN );
    glEnd();

    // $$$$ ABR 2011-04-21: scale and translate if meter unit
    const std::string geometry = template_->GetType().ascii();
    if( template_->GetUnit() == gui::DrawingTemplate::eMeter && geometry != "polygon" )
    {
        glScalef( SYMBOL_SCALE_RATIO_FOR_METER, SYMBOL_SCALE_RATIO_FOR_METER, 0.f );
        glTranslatef( SYMBOL_ICON_SIZE / 2.f / SYMBOL_SCALE_RATIO_FOR_METER, SYMBOL_ICON_SIZE / 5.f / SYMBOL_SCALE_RATIO_FOR_METER, 0.f );
    }

    // $$$$ ABR 2011-04-21: draw icon
    if( geometry == "polygon" )
        DrawOnPolygon();
    else if( geometry == "line" )
        DrawOnLine();
    else if( geometry == "point" )
        DrawOnPoint();

    glPopMatrix();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::DrawOnPoint
// Created: ABR 2011-04-20
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::DrawingInfo::DrawOnPoint()
{
    T_PointVector points;
    points.push_back( geometry::Point2f( 0.f, 0.f ) );
    DrawItem( points );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::DrawOnLine
// Created: ABR 2011-04-20
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::DrawingInfo::DrawOnLine()
{
    T_PointVector points;
    points.push_back( geometry::Point2f(                    SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE / 2.f ) );
    points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE / 2.f ) );
    DrawItem( points );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::DrawOnPolygon
// Created: ABR 2011-04-20
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::DrawingInfo::DrawOnPolygon()
{
    T_PointVector points;
    points.push_back( geometry::Point2f(                    SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN ) );
    points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN ) );
    points.push_back( geometry::Point2f( SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN,                    SYMBOL_ICON_MARGIN ) );
    points.push_back( geometry::Point2f(                    SYMBOL_ICON_MARGIN,                    SYMBOL_ICON_MARGIN ) );
    points.push_back( geometry::Point2f(                    SYMBOL_ICON_MARGIN, SYMBOL_ICON_SIZE - SYMBOL_ICON_MARGIN ) );
    DrawItem( points );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::SymbolInfo::DrawItem
// Created: ABR 2011-04-20
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::DrawingInfo::DrawItem( const T_PointVector& points )
{
    svg::RenderingContext context;
    context.SetViewport( geometry::BoundingBox( 0, 0, SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE ), SYMBOL_ICON_SIZE, SYMBOL_ICON_SIZE );
    svg::Color svgColor( "blue" );
    context.PushProperty( svg::RenderingContext_ABC::color, svgColor );
    if( points.size() == 1 )
        template_->Draw( points[ 0 ], context, tools_ );
    else
        template_->Draw( points, context, tools_ );
    context.PopProperty( svg::RenderingContext_ABC::color );
}


// -----------------------------------------------------------------------------
// ADN_Drawings_Data
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data constructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Drawings_Data::ADN_Drawings_Data()
    : ADN_Data_ABC()
    , renderer_()
    , tools_   ( new GlToolsSymbols() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data destructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Drawings_Data::~ADN_Drawings_Data()
{
    delete tools_;
    for( IT_DrawingsMap it = drawingsMap_.begin(); it != drawingsMap_.end(); ++it )
        it->second.clear();
    drawingsMap_.clear();
    for( IT_DrawingInfoVector it = drawings_.begin(); it != drawings_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::FilesNeeded
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::FilesNeeded( T_StringList& /*files*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::Load
// Created: ABR 2011-04-22
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::Load( const tools::Loader_ABC& fileLoader )
{
    const std::string filename = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDrawingTemplates_.GetData();
    fileLoader.LoadFile( filename, boost::bind( &ADN_Drawings_Data::ReadArchive, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::Reset
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::Reset()
{
    drawings_.Reset();
    drawingsMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadArchive
// Created: ABR 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadArchive( xml::xistream& xis )
{
    xis >> xml::start( "templates" )
            >> xml::list( "category", *this, &ADN_Drawings_Data::ReadCategory );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadCategory
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadCategory( xml::xistream& xis )
{
    bool hidden = xis.attribute< bool >( "hidden", false );
    if( hidden ) // $$$$ ABR 2011-04-22: check for hidden to display only tactical graphics category
        xis >> xml::list( "template", *this, &ADN_Drawings_Data::ReadTemplate );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadTemplate
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadTemplate( xml::xistream& xis )
{
    drawings_.AddItem( new DrawingInfo( xis, renderer_, *tools_ ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetSymbol
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
ADN_Drawings_Data::DrawingInfo* const ADN_Drawings_Data::GetDrawing( const std::string& code ) const
{
    for( CIT_DrawingInfoVector it = drawings_.begin(); it != drawings_.end(); ++it )
        if( (*it)->GetCode() == code )
            return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetSymbols
// Created: ABR 2011-04-21
// -----------------------------------------------------------------------------
ADN_Drawings_Data::T_DrawingInfoVector& ADN_Drawings_Data::GetDrawings( const std::string geometries )
{
    T_DrawingInfoVector& currentVector = drawingsMap_[ geometries ];
    if( currentVector.empty() )
    {
        QStringList qlist = QStringList::split( ',', geometries.c_str() );

        for( IT_DrawingInfoVector it = drawings_.begin(); it != drawings_.end(); ++it )
            if( (*it)->GetGeometry() == geometries )
                currentVector.AddItem( *it );
    }
    return currentVector;
}
