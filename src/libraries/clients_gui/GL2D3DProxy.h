// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlProxy_h_
#define __GlProxy_h_

#include "GLProxyBase.h"
#include "LayersHelpers.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class EntityResolver_ABC;
    class Filter_ABC;
    class GraphicalEntity_ABC;
    class StaticModel;
    class Options;
    class OptionsController;
    class Profile_ABC;
    class Settings;
}

namespace tools
{
    class ExerciseConfig;
}

class Lighting_ABC;
class EventStrategy_ABC;

namespace gui
{
    class DrawingTypes;
    class GLStackedWidget;
    class GlRenderPass_ABC;
    class GL2DWidget;
    class GL3DWidget;
    class IconLayout;
    class Layer;
    class TooltipsLayer_ABC;

// =============================================================================
/** @class  GL2D3DProxy
@brief  GL2D3DProxy
*/
// Created: AGE 2006-03-29
// =============================================================================
class GL2D3DProxy : public GLProxyBase
{
public:
    //! @name Constructors/Destructor
    //@{
    // create a GL2D3DProxy sharing registry options with OptionsController
    GL2D3DProxy( GLView_ABC& parent,
                 kernel::Controllers& controllers,
                 const kernel::Profile_ABC& profile,
                 const kernel::StaticModel& staticModel,
                 const kernel::EntityResolver_ABC& model,
                 const std::shared_ptr< Lighting_ABC >& lighting,
                 const QString& name,
                 const unsigned id );
    // create a GL2D3DProxy with a clone of the given GLView
    GL2D3DProxy( GLView_ABC& parent,
                 const GLView_ABC& other,
                 const QString& name,
                 const unsigned id );
    virtual ~GL2D3DProxy();
    //@}

    //! @name Informations -> implementation
    //@{
    virtual const QString& GetName() const;
    virtual unsigned GetID() const;
    //@}

    //! @name View -> implementation
    //@{
    virtual void Register( const T_View& view );
    virtual void Unregister( const T_View& view );

    virtual GLView_ABC& GetActiveView();
    virtual const GLView_ABC& GetActiveView() const;
    virtual GLView_ABC& GetCurrentView();
    virtual const GLView_ABC& GetCurrentView() const;
    virtual GLView_ABC& GetHoveredView();
    virtual const GLView_ABC& GetHoveredView() const;
    //@}

    //! @name View setters -> forward to parent
    //@{
    virtual void SetActiveView( GLView_ABC& view );
    virtual void SetCurrentView( GLView_ABC* view );
    virtual void SetHoveredView( GLView_ABC* view );
    //@}

    //! @name Options -> implementation
    //@{
    virtual void ApplyOptions();

    virtual GLOptions& GetActiveOptions();
    virtual const GLOptions& GetActiveOptions() const;
    virtual GLOptions& GetCurrentOptions();
    virtual const GLOptions& GetCurrentOptions() const;
    //@}

    //! @name Layers registration -> implementation & forward to all children
    //@{
    virtual void AddLayers( const T_LayersVector& layers );
    virtual void RemoveLayer( const T_Layer& layer );
    //@}

    //! @name Layers order -> implementation & forward to all children
    //@{
    virtual bool MoveBelow( const std::shared_ptr< Layer_ABC >& lhs,
                            const std::shared_ptr< Layer_ABC >& rhs );
    //@}

    //! @name Layers helpers -> implementation
    //@{
    virtual const T_LayersVector& GetLayers() const;
    virtual bool IsInAReadOnlyLayer( const kernel::GraphicalEntity_ABC& selectable ) const;
    virtual void UpdateLayerOrder();
    //@}

    //! @name Common picking -> forward to parent
    //@{
    virtual PickingSelector& GetPickingSelector();
    virtual const PickingSelector& GetPickingSelector() const;
    virtual void RenderPicking( const T_ObjectPicking& object );
    //@}

    //! @name Tooltip creator -> throw not implemented
    //@{
    virtual std::unique_ptr< GlTooltip_ABC > GL2D3DProxy::CreateTooltip() const;
    //@}

    //! @name Common drawing -> forward to parent
    ColorStrategy_ABC& GetColorStrategy() const;
    virtual float GetCurrentAlpha() const;
    virtual void SetCurrentColor( float r, float g, float b, float a = 1 );

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
    //@}

private:
    //! @name Member data
    //@{
    const QString name_;
    const unsigned id_;
    GLView_ABC& parent_;
    std::unique_ptr< GLOptions > options_;
    T_LayersVector layers_;
    //@}
};

} //! namespace gui

#endif // __GlProxy_h_
