// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GLMainProxy.h"

#include "AutomatsLayer.h"
#include "ContourLinesComputer.h"
#include "ContourLinesObserver.h"
#include "FormationLayer.h"
#include "GLOptions.h"
#include "GLSymbols.h"
#include "GlTooltip.h"
#include "PickingSelector.h"
#include "SignalAdapter.h"
#include "SvglProxy.h"
#include "SvglRenderer.h"
#include "TacticalGraphics.h"
#include "TooltipsLayer_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Filter_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GLMainProxy constructor
// Created: ABR 2014-06-23
// -----------------------------------------------------------------------------
GLMainProxy::GLMainProxy( kernel::Controllers& controllers,
                          kernel::Filter_ABC& profileFilter,
                          const tools::Resolver< kernel::Formation_ABC >& formationResolver )
    : controllers_( controllers )
    , profileFilter_( profileFilter )
    , formationResolver_( formationResolver )
    , colorStrategy_( 0 )
    , billboard_( 0 )
    , alpha_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GLMainProxy destructor
// Created: ABR 2014-06-23
// -----------------------------------------------------------------------------
GLMainProxy::~GLMainProxy()
{
    for( auto it = icons_.begin(); it != icons_.end(); ++it )
        glDeleteTextures( 1, &it->second );
    glDeleteLists( billboard_, 1 );
}

// -----------------------------------------------------------------------------
// MainProxy operations
// -----------------------------------------------------------------------------
void GLMainProxy::Load( const DrawingTypes& drawingTypes )
{
    renderer_.reset( new SvglRenderer() );
    svgl_.reset( new SvglProxy( *renderer_ ) );
    symbols_.reset( new GLSymbols( *renderer_ ) );
    graphics_.reset( new TacticalGraphics( drawingTypes  ) );
    pickingSelector_.reset( new PickingSelector() );
    contourLinesObserver_.reset( new ContourLinesObserver() );
    GetActiveOptions().Load();
    UpdateLayerOrder();
}

void GLMainProxy::Purge()
{
    GetActiveOptions().Purge();
    currentView_.reset();
    hoveredView_.reset();
    contourLinesObserver_.reset();
    pickingSelector_.reset();
    graphics_.reset();
    symbols_.reset();
    svgl_.reset();
    renderer_.reset();
}

void GLMainProxy::AddActiveChangeObserver( QObject* parent, const T_GLObserver& observer )
{
    gui::connect( parent, SIGNAL( destroyed( QObject* ) ), [=](){
        activeChangeObservers_.erase( parent );
    } );
    activeChangeObservers_[ parent ] = observer;
    if( activeView_ )
        observer( activeView_ );
}

void GLMainProxy::AddCreationObserver( QObject* parent, const T_GLObserver& observer )
{
    gui::connect( parent, SIGNAL( destroyed( QObject* ) ), [ = ]() {
        creationObservers_.erase( parent );
    } );
    creationObservers_[ parent ] = observer;
    std::for_each( views_.begin(), views_.end(), [=]( const T_View& view ) {
        observer( view );
    } );
}

void GLMainProxy::AddDeletionObserver( QObject* parent, const T_GLObserver& observer )
{
    gui::connect( parent, SIGNAL( destroyed( QObject* ) ), [=](){
        deletionObservers_.erase( parent );
    } );
    deletionObservers_[ parent ] = observer;
}

void GLMainProxy::ApplyToOptions( const std::function< void( GLOptions& ) >& functor )
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        functor( ( *it )->GetCurrentOptions() );
}

void GLMainProxy::ApplyToOptions( const std::function< void( const GLOptions& ) >& functor ) const
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        functor( ( *it )->GetCurrentOptions() );
}

void GLMainProxy::ApplyToViews( const T_GLObserver& functor ) const
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        functor( *it );
}

GLView_ABC::T_View GLMainProxy::GetView( unsigned id )
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        if( ( *it )->GetID() == id )
            return *it;
    return T_View();
}

size_t GLMainProxy::Count() const
{
    return views_.size();
}

std::shared_ptr< ContourLinesComputer > GLMainProxy::GetContourLinesComputer( int height ) const
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
    {
        const auto& options = ( *it )->GetActiveOptions();
        if( const auto& computer = options.GetContourLinesComputer() )
            if( options.Get( "ContourLines/Enabled" ).To< bool >() && computer->GetHeight() == height )
                return computer;
    }
    return std::shared_ptr< ContourLinesComputer >();
}

void GLMainProxy::SetColorStrategy( ColorStrategy_ABC& colorStrategy )
{
    colorStrategy_ = &colorStrategy;
}

void GLMainProxy::SetTooltipsLayer( const std::shared_ptr< TooltipsLayer_ABC >& layer )
{
    tooltipLayer_ = layer;
}

// -----------------------------------------------------------------------------
// Informations -> throw not implemented
// -----------------------------------------------------------------------------
const QString& GLMainProxy::GetName() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

unsigned GLMainProxy::GetID() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Proxy -> implementation
// -----------------------------------------------------------------------------
void GLMainProxy::Register( const std::shared_ptr< GLView_ABC >& view )
{
    GLProxyBase::Register( view );
    if( !activeView_ )
        activeView_ = view;
    std::for_each( creationObservers_.begin(),
                   creationObservers_.end(),
                   [=]( const std::pair< QObject*, T_GLObserver >& observer ) { observer.second( view ); } );
}

void GLMainProxy::Unregister( const std::shared_ptr< GLView_ABC >& view )
{
    GLProxyBase::Unregister( view );
    if( currentView_ == view )
        currentView_.reset();
    if( hoveredView_ == view )
        hoveredView_.reset();
    if( activeView_ == view && defaultView_ )
        SetActiveView( *defaultView_ );
    std::for_each( deletionObservers_.begin(),
                   deletionObservers_.end(),
                   [=]( const std::pair< QObject*, T_GLObserver >& observer ) { observer.second( view ); } );
}

namespace
{
    template< typename T >
    GLView_ABC& GetView( T& targetedView,
                         T& defaultView,
                         const std::string& errorType )
    {
        if( targetedView )
            return *targetedView;
        if( defaultView )
            return *defaultView;
        throw MASA_EXCEPTION( "cannot get " + errorType + " view." );
    }
}

GLView_ABC& GLMainProxy::GetCurrentView()
{
    return ::GetView( currentView_, defaultView_, "current" );
}

const GLView_ABC& GLMainProxy::GetCurrentView() const
{
    return ::GetView( currentView_, defaultView_, "current" );
}

GLView_ABC& GLMainProxy::GetActiveView()
{
    return ::GetView( activeView_, defaultView_, "active" );
}

const GLView_ABC& GLMainProxy::GetActiveView() const
{
    return ::GetView( activeView_, defaultView_, "active" );
}

GLView_ABC& GLMainProxy::GetHoveredView()
{
    return ::GetView( hoveredView_, defaultView_, "hovered" );
}

const GLView_ABC& GLMainProxy::GetHoveredView() const
{
    return ::GetView( hoveredView_, defaultView_, "hovered" );
}

// -----------------------------------------------------------------------------
// View setters -> implementation
// -----------------------------------------------------------------------------
namespace
{
    std::shared_ptr< GLView_ABC > FindView( const std::vector< GLView_ABC::T_View >& views,
                                            GLView_ABC* searchedView,
                                            const std::string& error )
    {
        if( !searchedView )
            return std::shared_ptr< GLView_ABC >();
        auto it = std::find_if( views.begin(),
                                views.end(),
                                [ searchedView ]( const std::shared_ptr< GLView_ABC >& view ) {
            return view.get() == searchedView;
        } );
        if( it == views.end() )
            throw MASA_EXCEPTION( error );
        return *it;
    }
}

void GLMainProxy::SetActiveView( GLView_ABC& view )
{
    if( activeView_ && activeView_.get() == &view )
        return;
    activeView_ = FindView( views_, &view, "Unable to find the active view" );
    const auto& options = activeView_->GetActiveOptions();
    // we set the controller's view options first so active changes observers 
    // can access an option with the new value
    controllers_.options_.SetViewOptions( options.GetOptions() );
    controllers_.options_.UpdateViewOptions();
    profileFilter_.SetFilter( options.GetFilterEntity(), true );
    std::for_each( activeChangeObservers_.begin(),
                   activeChangeObservers_.end(),
                   [=]( const std::pair< QObject*, T_GLObserver >& observer ) { observer.second( activeView_ ); } );
    // reset contour lines observer
    ApplyToOptions( []( GLOptions& options ) {
        options.GetContourLinesComputer()->SetContourLinesObserver( std::shared_ptr< ContourLinesObserver >() );
    } );
    GetActiveOptions().GetContourLinesComputer()->SetContourLinesObserver( contourLinesObserver_ );
}

void GLMainProxy::SetCurrentView( GLView_ABC* view )
{
    currentView_ = FindView( views_, view, "Unable to find the current view" );
}

void GLMainProxy::SetHoveredView( GLView_ABC* view )
{
    hoveredView_ = FindView( views_, view, "Unable to find the hovered view" );
}

// -----------------------------------------------------------------------------
// Options -> implementation
// -----------------------------------------------------------------------------
namespace
{
    void ToggleAggregation( const kernel::Entity_ABC& entity, bool aggregate )
    {
        auto hierarchy = entity.Retrieve< kernel::TacticalHierarchies >();
        if( !hierarchy )
            return;
        auto it = hierarchy->CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            auto& child = const_cast< kernel::Entity_ABC& >( it.NextElement() );
            child.SetAggregatedSubordinate( aggregate );
            ToggleAggregation( child, aggregate );
        }
    }
}

void GLMainProxy::ApplyOptions()
{
    auto& currentView = GetCurrentView();
    auto& options = currentView.GetCurrentOptions();
    // locked entity
    if( auto entity = options.GetLockedEntity() )
        if( const auto* position = entity->Retrieve< kernel::Positions >() )
            currentView.CenterOn( position->GetPosition( false ) );
    // orbat filter
    profileFilter_.SetFilter( options.GetFilterEntity(), false );
    // layers alpha
    auto& layers = currentView.GetLayers();
    std::for_each( layers.begin(), layers.end(), [&options]( const T_Layer& layer ) -> bool {
        const std::string name = "Layers/" + layer->GetOptionName() + "/Alpha";
        if( options.Has( name ) )
            layer->SetAlpha( options.Get( name ).To< float >() );
        return false;
    } );
    // Aggregation level
    formationResolver_.Apply( []( const kernel::Formation_ABC& formation ){
        ToggleAggregation( formation, false );
    } );
    const auto& entities = options.GetAggregatedEntities();
    for( auto entity = entities.begin(); entity != entities.end(); ++entity )
            ToggleAggregation( **entity, true );
}

GLOptions& GLMainProxy::GetActiveOptions()
{
    if( !activeView_ )
        throw MASA_EXCEPTION( "No active view defined" );
    return activeView_->GetActiveOptions();
}

const GLOptions& GLMainProxy::GetActiveOptions() const
{
    if( !activeView_ )
        throw MASA_EXCEPTION( "No active view defined" );
    return activeView_->GetActiveOptions();
}

GLOptions& GLMainProxy::GetCurrentOptions()
{
    return GetCurrentView().GetCurrentOptions();
}

const GLOptions& GLMainProxy::GetCurrentOptions() const
{
    return GetCurrentView().GetCurrentOptions();
}

// -----------------------------------------------------------------------------
// Layers registration -> overload
// -----------------------------------------------------------------------------
void GLMainProxy::RemoveLayer( const T_Layer& layer )
{
    GLProxyBase::RemoveLayer( layer );
    if( tooltipLayer_ == layer )
        tooltipLayer_.reset();
}

// -----------------------------------------------------------------------------
// Layers order -> forward to active view
// -----------------------------------------------------------------------------
bool GLMainProxy::MoveBelow( const T_Layer& lhs,
                             const T_Layer& rhs )
{
    return GetActiveView().MoveBelow( lhs, rhs );
}

// -----------------------------------------------------------------------------
// Layers helpers -> forward to current view
// -----------------------------------------------------------------------------
const T_LayersVector& GLMainProxy::GetLayers() const
{
    return GetCurrentView().GetLayers();
}

bool GLMainProxy::IsInAReadOnlyLayer( const kernel::GraphicalEntity_ABC& selectable ) const
{
    return GetCurrentView().IsInAReadOnlyLayer( selectable );
}

void GLMainProxy::UpdateLayerOrder()
{
    for( auto it = views_.begin(); it != views_.end(); ++it )
        ( *it )->UpdateLayerOrder();
}

// -----------------------------------------------------------------------------
// Picking -> implementation
// -----------------------------------------------------------------------------
PickingSelector& GLMainProxy::GetPickingSelector()
{
    return *pickingSelector_;
}

const PickingSelector& GLMainProxy::GetPickingSelector() const
{
    return *pickingSelector_;
}

void GLMainProxy::RenderPicking( const T_ObjectPicking& object )
{
    pickingSelector_->RenderPicking( object, boost::bind( &GLMainProxy::SetCurrentColor, this, _1, _2, _3, _4 ) );
}

// -----------------------------------------------------------------------------
// Tooltip creator -> implementation
// -----------------------------------------------------------------------------
std::unique_ptr< GlTooltip_ABC > GLMainProxy::CreateTooltip() const
{
    if( tooltipLayer_ )
        return std::unique_ptr< GlTooltip_ABC >( new GlTooltip( *tooltipLayer_ ) );
    throw MASA_EXCEPTION( "ToolTipLayer not set" );
}

// -----------------------------------------------------------------------------
// Common drawing -> implementation
// -----------------------------------------------------------------------------
ColorStrategy_ABC& GLMainProxy::GetColorStrategy() const
{
    if( colorStrategy_ )
        return *colorStrategy_;
    throw MASA_EXCEPTION( "Color strategy not set" );
}

void GLMainProxy::SetCurrentColor( float r, float g, float b, float a /* = 1 */ )
{
    glColor4f( r, g, b, a );
    renderer_->SetCurrentColor( r, g, b, a );
    graphics_->SetCurrentColor( r, g, b, a );
    alpha_ = a;
}

float GLMainProxy::GetCurrentAlpha() const
{
    return alpha_;
}

void GLMainProxy::BindIcon( const char** xpm ) const
{
    unsigned int& texture = const_cast< GLMainProxy* >( this )->icons_[ xpm ];
    if( !texture )
    {
        QImage image( xpm );
        if( !image.isNull() )
        {
            image = image.convertDepth( 32 );
            int nTrueWidth = image.width();
            int nTrueHeight = image.height();
            int nWidth = 2;
            int nHeight = 2;
            while( nWidth < nTrueWidth )
                nWidth *= 2;
            while( nHeight < nTrueHeight )
                nHeight *= 2;

            QImage resizedImage = image.copy( 0, 0, nWidth, nHeight );
            glGenTextures( 1, &texture );
            glBindTexture( GL_TEXTURE_2D, texture );
            glTexImage2D( GL_TEXTURE_2D, 0, 4, resizedImage.width(), resizedImage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, resizedImage.bits() );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        }
    }
    glBindTexture( GL_TEXTURE_2D, texture );
}

namespace
{
    void DrawQuad( float x1, float y1, float x2, float y2 )
    {
        glBegin( GL_QUADS );
        glVertex3f( x1, y1, -1 );
        glVertex3f( x2, y1, -1 );
        glVertex3f( x2, y2, -1 );
        glVertex3f( x1, y2, -1 );
        glEnd();
    }
}

void GLMainProxy::DrawActiveFrame() const
{
    if( currentView_ != activeView_ || views_.size() == 1 )
        return;

    int viewport[ 4 ];
    glGetIntegerv( GL_VIEWPORT, viewport );
    const float size = 10.f;
    const float x = size / viewport[ 2 ];
    const float y = size / viewport[ 3 ];

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    const QColor color = controllers_.options_.GetGeneralOptions()->Get( "Color/ActiveViewFrame" ).To< QString >();
    glColor3f( static_cast< float >( color.redF() ),
               static_cast< float >( color.greenF() ),
               static_cast< float >( color.blueF() ) );
    DrawQuad( 1 - x, -1.f, 1.f, 1.f );
    DrawQuad( -1.f + x, 1 - y, 1.f - x, 1.f );
    DrawQuad( -1.f, -1.f, -1 + x, 1.f );
    DrawQuad( -1.f + x, -1.f, 1.f - x, -1 + y );

    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
}

void GLMainProxy::DrawBillboardRect() const
{
    if( !billboard_ )
    {
        const_cast< GLMainProxy* >( this )->billboard_ = glGenLists( 1 );
        glNewList( billboard_, GL_COMPILE );
        glBegin( GL_QUADS );
        glTexCoord2f( 0.f, 1.f );
        glVertex2f( -1, -1 );
        glTexCoord2f( 1.f, 1.f );
        glVertex2f( 1, -1 );
        glTexCoord2f( 1.f, 0.f );
        glVertex2f( 1, 1 );
        glTexCoord2f( 0.f, 0.f );
        glVertex2f( -1, 1 );
        glEnd();
        glEndList();
    }
    glCallList( billboard_ );
}

void GLMainProxy::PrintApp6( const std::string& symbol,
                             const std::string& style,
                             const geometry::Rectangle2f& viewport,
                             unsigned vWidth /* = 640 */,
                             unsigned vHeight /* = 480 */ ) const
{
    symbols_->PrintApp6( symbol,
                         style,
                         viewport,
                         vWidth,
                         vHeight,
                         pickingSelector_->IsPickingMode() );
}

void GLMainProxy::DrawSvgInViewport( const std::string& filename,
                           const geometry::Rectangle2f& viewport,
                           unsigned vWidth /*= 640*/,
                           unsigned vHeight /*= 480*/ ) const
{
    svgl_->Draw( filename, viewport, vWidth, vHeight );
}

void GLMainProxy::DrawTacticalGraphics( const std::string& symbol,
                                        const kernel::Location_ABC& location,
                                        const geometry::Rectangle2f& viewport,
                                        bool overlined,
                                        float pointSize,
                                        float zoom ) const
{
    graphics_->Draw( symbol,
                     location,
                     viewport,
                     GetCurrentView(),
                     overlined,
                     pointSize,
                     zoom );
}
