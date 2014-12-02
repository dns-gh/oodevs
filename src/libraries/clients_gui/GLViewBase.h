// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef GLViewBase_h_
#define GLViewBase_h_

#include "GLView_ABC.h"

namespace gui
{
    class PickingSelector;

// =============================================================================
/** @class  GLViewBase
    @brief  GLViewBase
*/
// Created: AGE 2006-04-07
// =============================================================================
class GLViewBase : public GLView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GLViewBase( GLView_ABC& parent );
    virtual ~GLViewBase();
    //@}

    //! @name Informations -> throw not implemented
    //@{
    virtual const QString& GetName() const;
    virtual unsigned GetID() const;
    //@}

    //! @name Proxy -> throw not implemented
    //@{
    virtual void Register( const std::shared_ptr< GLView_ABC >& );
    virtual void Unregister( const std::shared_ptr< GLView_ABC >& );
    virtual GLView_ABC& GetCurrentView();
    virtual const GLView_ABC& GetCurrentView() const;
    virtual GLView_ABC& GetActiveView();
    virtual const GLView_ABC& GetActiveView() const;
    virtual T_View GetHoveredView() const;
    //@}

    //! @name View setters -> forward to parent
    //@{
    virtual void SetActiveView( GLView_ABC& view );
    virtual void SetCurrentView( GLView_ABC* view );
    virtual void SetHoveredView( GLView_ABC* view );
    //@}

    //! @name Options -> forward to parent
    //@{
    virtual void ApplyOptions();

    virtual GLOptions& GetActiveOptions();
    virtual const GLOptions& GetActiveOptions() const;
    virtual GLOptions& GetCurrentOptions();
    virtual const GLOptions& GetCurrentOptions() const;
    //@}

    //! @name Layers -> throw not implemented
    //@{
    virtual const T_LayersVector& GetLayers() const;
    virtual T_Layer GetLayer( const T_LayerFunctor& functor ) const;
    virtual void UpdateLayerOrder();
    //@}

    //! @name Picking -> forward to parent
    //@{
    virtual PickingSelector& GetPickingSelector();
    virtual const PickingSelector& GetPickingSelector() const;
    virtual void RenderPicking( const T_ObjectPicking& object );
    //@}

    //! @name Tooltip creator -> throw not implemented
    //@{
    virtual std::unique_ptr< GlTooltip_ABC > CreateTooltip() const;
    //@}

    //! @name Common drawing -> forward to parent
    //@{
    virtual ColorStrategy_ABC& GetColorStrategy() const;
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
                            unsigned vHeight = 480 ) const;
    //@}

protected:
    //! @name Member data
    //@{
    GLView_ABC& parent_;
    QGLContext* context_;
    //@}
};

} //! namespace gui

#endif // GLViewBase_h_
