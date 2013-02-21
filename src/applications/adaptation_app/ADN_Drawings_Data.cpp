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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/GlTooltip.h"
#include "clients_gui/TooltipsLayer.h"
#include "svgl/Color.h"
#include "svgl/RenderingContext.h"
#include "svgl/TextRenderer.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>

namespace
{
    class GlTooltip_ABC;

    class GlToolsSymbols : public gui::GlTools_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 GlToolsSymbols() : GlTools_ABC() {}
        virtual ~GlToolsSymbols() {}
        //@}

        //! @name Options
        //@{
        virtual boost::tuple< bool, bool, bool > UnSelect() const { return boost::tuple< bool, bool, bool >( false, false, false ); }
        virtual void Select( bool, bool, bool ) const {}
        virtual bool ShouldDisplay( const std::string& ) const { return true; }
        virtual bool ShouldDisplay( const std::string&, bool ) const { return true; }
        //@}

        //! @name Accessors
        //@{
        virtual float           Pixels( const geometry::Point2f& ) const { return 0.001f; } // $$$$ ABR 2011-04-21: hard coded
        virtual unsigned short  StipplePattern( int ) const { return 0; }
        virtual float           Zoom() const { return 0.f; }
        virtual float           GetAdaptiveZoomFactor( bool ) const { return 0.f; }
        //@}

        //! @name Operations
        //@{
        virtual std::auto_ptr< kernel::GlTooltip_ABC > CreateTooltip() const { return std::auto_ptr< kernel::GlTooltip_ABC >( 0 ); }
        virtual void SetCurrentColor  ( float, float, float, float ) {}
        virtual void SetCurrentCursor ( const QCursor& ) {}
        virtual void DrawCross        ( const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawLine         ( const geometry::Point2f&, const geometry::Point2f& ) const {}
        virtual void DrawStippledLine ( const geometry::Point2f&, const geometry::Point2f& ) const {}
        virtual void DrawLines        ( const T_PointVector& ) const {}
        virtual void DrawRectangle    ( const T_PointVector& ) const {}
        virtual void DrawPolygon      ( const T_PointVector& ) const {}
        virtual void DrawSelectedPolygon( const T_PointVector& ) const {}
        virtual void DrawDecoratedPolygon( const geometry::Polygon2f&, const kernel::UrbanColor_ABC&, const std::string&, unsigned int, unsigned int, bool ) const {}
        virtual void DrawConvexDecoratedPolygon( const geometry::Polygon2f&, const kernel::UrbanColor_ABC&, const std::string&, unsigned int, bool ) const {}
        virtual void DrawArrow        ( const geometry::Point2f&, const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawCurvedArrow  ( const geometry::Point2f&, const geometry::Point2f&, float, float, E_Unit ) const {}
        virtual void DrawArc          ( const geometry::Point2f&, const geometry::Point2f&, const geometry::Point2f& ) const {}
        virtual void DrawCircle       ( const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawDisc         ( const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawLife         ( const geometry::Point2f&, float, float, bool ) const {}
        virtual void Print            ( const std::string&, const geometry::Point2f& ) const {}
        virtual void Print            ( const std::string&, const geometry::Point2f&, const QFont& ) const {}
        virtual void DrawApp6Symbol   ( const std::string&, const geometry::Point2f&, float, float ) const {}
        virtual void DrawApp6Symbol   ( const std::string&, const std::string&, const geometry::Point2f&, float, float ) const {}
        virtual void DrawIcon         ( const char**, const geometry::Point2f&, float, E_Unit ) const {}
        virtual void DrawImage        ( const QImage&, const geometry::Point2f& ) const {}
        virtual void DrawCell         ( const geometry::Point2f& ) const {}
        virtual void DrawSvg          ( const std::string&, const geometry::Point2f&, float ) const {}
        virtual void DrawTacticalGraphics( const std::string&, const kernel::Location_ABC&, bool, bool ) const {}
        virtual void DrawApp6SymbolFixedSize( const std::string&, const geometry::Point2f&, float) const {}
        virtual void FillSelection( const geometry::Point2f&, T_ObjectsPicking& ) {};
        virtual void RegisterObjectPicking( const T_ObjectPicking& ) {};
        virtual geometry::Rectangle2f GlobalViewport() const { return geometry::Rectangle2f(); };
        virtual bool IsPickingMode() const { return false; };
        //@}
    };
}

// -----------------------------------------------------------------------------
// ADN_Drawings_Data
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data constructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Drawings_Data::ADN_Drawings_Data()
    : ADN_Data_ABC( eDrawings )
    , renderer_( new svg::TextRenderer() )
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
    for( auto it = geometryMap_.begin(); it != geometryMap_.end(); ++it )
        it->second.clear();
    geometryMap_.clear();
    for( auto it = categoryMap_.begin(); it != categoryMap_.end(); ++it )
        it->second.clear();
    categoryMap_.clear();
    for( auto it = drawings_.begin(); it != drawings_.end(); ++it )
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
    geometryMap_.clear();
    categoryMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadArchive
// Created: ABR 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadArchive( xml::xistream& xis )
{
    xis >> xml::start( "templates" )
            >> xml::list( "category", *this, &ADN_Drawings_Data::ReadCategory );
    xml::xistringstream xss( "<template name=' - ' type='default'>"
                             "    <segment/>"
                             "</template>" );

    std::auto_ptr< DrawingInfo > drawing;
    drawing.reset( new DrawingInfo( xss >> xml::start( "template" ), "tasks", "tasks", *renderer_ ) );
    drawing->GenerateSamplePixmap( *tools_ );
    drawings_.AddItem( drawing.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadCategory
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadCategory( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name", "" );
    const std::string id = xis.attribute< std::string >( "id", "" );
    if( id != "" ) // $$$$ ABR 2011-04-22: check for hidden to display only tactical graphics category
        xis >> xml::list( "template", *this, &ADN_Drawings_Data::ReadTemplate, name, id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadTemplate
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadTemplate( xml::xistream& xis, const std::string& name, const std::string& id )
{
    try
    {
        std::auto_ptr< DrawingInfo > drawing;
        drawing.reset( new DrawingInfo( xis, name.c_str(), id.c_str(), *renderer_ ) );
        drawing->GenerateSamplePixmap( *tools_ );
        drawings_.AddItem( drawing.release() );
    }
    catch( const std::exception& e )
    {
        std::cout << "Error reading drawing template " << tools::GetExceptionMsg( e ) << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetSymbol
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
ADN_Drawings_Data::DrawingInfo* const ADN_Drawings_Data::GetDrawing( const std::string& code ) const
{
    for( auto it = drawings_.begin(); it != drawings_.end(); ++it )
        if( (*it)->GetCode() == code.c_str() )
            return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetGeometryDrawings
// Created: ABR 2011-04-21
// -----------------------------------------------------------------------------
ADN_Drawings_Data::T_DrawingInfoVector& ADN_Drawings_Data::GetGeometryDrawings( const std::string& geometries, const std::string& category )
{
    T_DrawingInfoVector& currentVector = geometryMap_[ geometries ];
    if( currentVector.empty() )
    {
        QRegExp reg( "[, ]" );
        QStringList qlist = QStringList::split( reg, geometries.c_str() );

        for( auto it = drawings_.begin(); it != drawings_.end(); ++it )
            if( (*it)->GetId() == category.c_str() && qlist.contains( (*it)->GetType() ) )
                currentVector.AddItem( *it );
    }
    return currentVector;
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetCategoryDrawings
// Created: LGY 2011-08-30
// -----------------------------------------------------------------------------
ADN_Drawings_Data::T_DrawingInfoVector& ADN_Drawings_Data::GetCategoryDrawings( const std::string& category )
{
    T_DrawingInfoVector& currentVector = categoryMap_[ category ];
    if( currentVector.empty() )
        for( auto it = drawings_.begin(); it != drawings_.end(); ++it )
            if( (*it)->GetId() == category.c_str() )
                currentVector.AddItem( *it );
    return currentVector;
}
