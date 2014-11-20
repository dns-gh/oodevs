// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef GLMainProxy_h
#define GLMainProxy_h

#include "GLProxyBase.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class Filter_ABC;
    class Formation_ABC;
    template< typename T > class Resolver;
}

namespace gui
{
    class ContourLinesComputer;
    class ContourLinesObserver;
    class DrawingTypes;
    class GLProxyBase;
    class GLSymbols;
    class PickingSelector;
    class SvglRenderer;
    class SvglProxy;
    class TacticalGraphics;
    class TooltipsLayer_ABC;

// =============================================================================
/** @class  GLMainProxy
    @brief  GLMainProxy
*/
// Created: ABR 2014-06-23
// =============================================================================
class GLMainProxy : public GLProxyBase
{
public:
    //! @name Types
    //@{
    typedef std::function< void( const T_View& ) > T_GLObserver;
    typedef tools::Map< QObject*, T_GLObserver >   T_GLObservers;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GLMainProxy( kernel::Controllers& controllers,
                          kernel::Filter_ABC& profileFilter,
                          const tools::Resolver< kernel::Formation_ABC >& formationResolver );
    virtual ~GLMainProxy();
    //@}

    //! @name Operations
    //@{
    void Load( const DrawingTypes& drawingTypes );
    void Purge();

    void AddActiveChangeObserver( QObject* parent, const T_GLObserver& observer );
    void AddCreationObserver( QObject* parent, const T_GLObserver& observer );
    void AddDeletionObserver( QObject* parent, const T_GLObserver& observer );

    void ApplyToOptions( const std::function< void( GLOptions& ) >& functor );
    void ApplyToOptions( const std::function< void( const GLOptions& ) >& functor ) const;
    void ApplyToViews( const T_GLObserver& functor ) const;
    T_View GetView( unsigned id );
    size_t Count() const;

    std::shared_ptr< ContourLinesComputer > GetContourLinesComputer( int height ) const;
    void SetColorStrategy( ColorStrategy_ABC& colorStrategy );
    void SetTooltipsLayer( const std::shared_ptr< TooltipsLayer_ABC >& layer );
    //@}

    //! @name Informations -> throw not implemented
    //@{
    virtual const QString& GetName() const;
    virtual unsigned GetID() const;
    //@}

    //! @name Proxy -> implementation
    //@{
    virtual void Register( const std::shared_ptr< GLView_ABC >& view );
    virtual void Unregister( const std::shared_ptr< GLView_ABC >& view );

    virtual GLView_ABC& GetActiveView();
    virtual const GLView_ABC& GetActiveView() const;
    virtual GLView_ABC& GetCurrentView();
    virtual const GLView_ABC& GetCurrentView() const;
    virtual GLView_ABC& GetHoveredView();
    virtual const GLView_ABC& GetHoveredView() const;

    virtual void SetActiveView( GLView_ABC& activeView );
    virtual void SetCurrentView( GLView_ABC* view );
    virtual void SetHoveredView( GLView_ABC* view );
    //@}

    //! @name Layers registration -> overload
    //@{
    virtual void RemoveLayer( const T_Layer& layer );
    //@}

    //! @name Layers order -> forward to active view
    //@{
    virtual bool MoveBelow( const T_Layer& lhs,
                            const T_Layer& rhs );
    //@}

    //! @name Layers helpers -> forward to current view
    //@{
    const T_LayersVector& GetLayers() const;
    virtual bool IsInAReadOnlyLayer( const kernel::GraphicalEntity_ABC& selectable ) const;
    virtual void UpdateLayerOrder();
    //@}

    //! @name Options -> forward to specific child
    //@{
    virtual void ApplyOptions();

    virtual GLOptions& GetActiveOptions();
    virtual const GLOptions& GetActiveOptions() const;
    virtual GLOptions& GetCurrentOptions();
    virtual const GLOptions& GetCurrentOptions() const;
    //@}

    //! @name Common picking -> implementation
    //@{
    virtual PickingSelector& GetPickingSelector();
    virtual const PickingSelector& GetPickingSelector() const;
    virtual void RenderPicking( const T_ObjectPicking& object );
    //@}

    //! @name Common drawing -> implementation
    //@{
    virtual ColorStrategy_ABC& GetColorStrategy() const;
    virtual void SetCurrentColor( float r, float g, float b, float a = 1 );
    virtual float GetCurrentAlpha() const;

    virtual void BindIcon( const char** xpm ) const;
    virtual void DrawActiveFrame() const;
    virtual void DrawBillboardRect() const;
    virtual void DrawSvgInViewport( const std::string& filename,
                                    const geometry::Rectangle2f& viewport,
                                    unsigned vWidth = 640,
                                    unsigned vHeight = 480 ) const;
    virtual void DrawTacticalGraphics( const std::string& symbol,
                                       const kernel::Location_ABC& location,
                                       const geometry::Rectangle2f& viewport,
                                       bool overlined,
                                       float pointSize,
                                       float zoom ) const;
    virtual void PrintApp6( const std::string& symbol,
                            const std::string& style,
                            const geometry::Rectangle2f& viewport,
                            unsigned vWidth = 640,
                            unsigned vHeight = 480,
                            bool checkAlpha = true ) const;

    virtual std::unique_ptr< GlTooltip_ABC > CreateTooltip() const;
    //@}

private:
    typedef std::map< const char**, unsigned int > T_Icons;

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Filter_ABC& profileFilter_;
    const tools::Resolver< kernel::Formation_ABC >& formationResolver_;
    std::shared_ptr< ContourLinesObserver > contourLinesObserver_;
    ColorStrategy_ABC* colorStrategy_;

    T_GLObservers activeChangeObservers_;
    T_GLObservers creationObservers_;
    T_GLObservers deletionObservers_;

    std::unique_ptr< SvglRenderer > renderer_;
    std::unique_ptr< SvglProxy > svgl_;
    std::unique_ptr< GLSymbols > symbols_;
    std::unique_ptr< TacticalGraphics > graphics_;
    std::unique_ptr< PickingSelector > pickingSelector_;

    std::shared_ptr< TooltipsLayer_ABC > tooltipLayer_;

    std::shared_ptr< GLView_ABC > activeView_;   // the view where the mouse was clicked last
    std::shared_ptr< GLView_ABC > currentView_;  // the view currently updating
    std::shared_ptr< GLView_ABC > hoveredView_;  // the view with mouse over it

    T_Icons icons_;
    unsigned int billboard_;
    float alpha_;
    //@}
};

} //! namespace gui

#endif // GLMainProxy_h
