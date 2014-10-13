// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Layer_h_
#define __Layer_h_

#include "Layer_ABC.h"
#include "LayersHelpers.h"
#include "clients_kernel/DisplayableModesObserver_ABC.h"
#include <tools/Observer_ABC.h>

class ViewFrustum;

namespace kernel
{
    class Controllers;
    class GraphicalEntity_ABC;
    class Logger_ABC;
}

namespace gui
{
    class GLView_ABC;
    class Viewport_ABC;

// =============================================================================
/** @class  Layer
    @brief  Layer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer : public Layer_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Layer( kernel::Controllers& controllers, GLView_ABC& view, E_LayerTypes type );
    virtual ~Layer();
    //@}

    //! @name ModesObserver implementation
    //@{
    virtual void SetVisible( bool visible );
    virtual void ForceEnabled( bool enabled );
    virtual void EnsureIsEnabled();
    virtual bool IsVisible() const;
    //@}

    //! @name Layer_ABC implementation
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Paint( Viewport_ABC& viewport );

    virtual void SetAlpha( float alpha );
    virtual float GetAlpha() const;

    virtual bool IsEnabled() const;
    virtual bool IsConfigurable() const;
    virtual void Reset();

    virtual bool IsIn( const kernel::GraphicalEntity_ABC& ) const { return false; }
    virtual E_LayerTypes GetType() const;
    virtual QString GetName() const;
    virtual std::string GetOptionName() const;
    virtual void Select( unsigned int id, bool control );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual bool ContextMenu( const std::vector< const kernel::GraphicalEntity_ABC* >&, const geometry::Point2f&, const QPoint& );
    virtual void FillContextMenu( QMouseEvent* mouse, kernel::ContextMenu& menu, const geometry::Point2f& point );
    virtual void FillContextMenu( unsigned int id, kernel::ContextMenu& menu );
    virtual void ExtractElements( T_LayerElements& extractedElement, const T_ObjectsPicking& selection );
    virtual bool ShowTooltip( const T_ObjectPicking& selection );
    virtual void HideTooltip();
    //@}

protected:
    //! @name Helpers
    //@{
    bool ShouldDrawPass() const;
    const std::vector< E_LayerTypes >& GetChildrenTypes() const;
    //@}

private:
    //! @name Helpers
    //@{
    bool IsPickable() const;
    //@}

protected:
    kernel::Controllers& controllers_;
    GLView_ABC& view_;
    QString name_;

private:
    const gui::layers::Descriptor& descriptor_;
    float alpha_;
    bool enabled_;
};

// =============================================================================
/** @class  Layer2D
    @brief  Layer2D
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer2D : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
            Layer2D( kernel::Controllers& controllers, GLView_ABC& view, E_LayerTypes type )
                 : Layer( controllers, view, type )
             {}
    virtual ~Layer2D() {};
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& ) {}
    //@}
};

// =============================================================================
/** @class  Layer3D
    @brief  Layer3D
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer3D : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
             Layer3D( kernel::Controllers& controllers, GLView_ABC& view, E_LayerTypes type )
                 : Layer( controllers, view, type )
             {}
    virtual ~Layer3D() {}
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& ) {}
    //@}
};

}

#endif // __Layer_h_
