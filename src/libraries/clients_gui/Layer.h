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
    class GlTools_ABC;
    class Viewport_ABC;

// =============================================================================
/** @class  Layer
    @brief  Layer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer : public QObject
            , public Layer_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Layer( kernel::Controllers& controllers, GlTools_ABC& tools, E_LayerTypes type );
    virtual ~Layer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Paint( Viewport_ABC& viewport );

    virtual void SetAlpha( float alpha );
    virtual float GetAlpha() const;

    virtual bool IsEnabled() const;
    virtual void Reset();

    void SetPasses( const std::string& passes );
    bool ShouldDrawPass() const;

    virtual bool IsIn( const kernel::GraphicalEntity_ABC& ) const { return false; }
    //@}

    //! @name ModesObserver implementation
    //@{
    virtual void SetVisible( bool visible );
    virtual void ForceEnabled( bool enabled );
    virtual void EnsureIsEnabled();
    virtual bool IsVisible() const;
    virtual bool IsPickable() const;
    //@}

    //! @name Layer_ABC implementation
    //@{
    virtual E_LayerTypes GetType() const;
    virtual QString GetName() const;
    virtual void Select( const kernel::GraphicalEntity_ABC&, bool control, bool shift );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual bool ContextMenu( const std::vector< const kernel::GraphicalEntity_ABC* >&, const geometry::Point2f&, const QPoint& );
    virtual void FillContextMenu( QMouseEvent* mouse, kernel::ContextMenu& menu, const geometry::Point2f& point );
    virtual void FillContextMenu( const kernel::GraphicalEntity_ABC& entity, kernel::ContextMenu& menu );
    virtual void ExtractElements( T_LayerElements& extractedElement, const T_ObjectsPicking& selection );
    virtual bool ShowTooltip( const T_ObjectPicking& selection );
    virtual void HideTooltip();
    //@}

protected:
    kernel::Controllers& controllers_;
    GlTools_ABC& tools_;

private:
    //! @name Member data
    //@{
    const E_LayerTypes type_;
    float         alpha_;
    std::string   passes_;
    bool          enabled_;
    //@}
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
             Layer2D( kernel::Controllers& controllers, GlTools_ABC& tools, E_LayerTypes type )
                 : Layer( controllers, tools, type )
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
             Layer3D( kernel::Controllers& controllers, GlTools_ABC& tools, E_LayerTypes type )
                 : Layer( controllers, tools, type )
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
