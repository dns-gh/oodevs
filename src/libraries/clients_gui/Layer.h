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
#include "tools/Observer_ABC.h"
#include "clients_kernel/DisplayableModesObserver_ABC.h"

class ViewFrustum;

namespace kernel
{
    class Controllers;
    class Selectable_ABC;
}

namespace gui
{
    class GlWidget;
    class Gl3dWidget;
    class Viewport_ABC;
}

namespace gui
{
// =============================================================================
/** @class  Layer
    @brief  Layer
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer : public Layer_ABC
            , public tools::Observer_ABC
            , public kernel::DisplayableModesObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Layer();
    virtual ~Layer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Paint( Viewport_ABC& viewport );

    virtual void RegisterIn( Gl3dWidget& widget );
    virtual void RegisterIn( GlWidget& widget );

    virtual void UnregisterIn( Gl3dWidget& widget );
    virtual void UnregisterIn( GlWidget& widget );

    virtual void Reset2d();
    virtual void Reset3d();

    virtual void SetAlpha( float alpha );
    float GetAlpha() const;

    bool IsEnabled() const;

    void MoveAbove( Layer& layer );
    void MoveBelow( Layer& layer );

    void SetPasses( const std::string& passes );
    std::string GetCurrentPass() const;
    bool ShouldDrawPass() const;

    virtual bool IsIn( const kernel::Selectable_ABC& ) const { return false; }
    //@}

    //! @name ModesObserver implementation
    //@{
    virtual void SetVisible( bool visible );
    virtual void ForceEnabled( bool enabled );
    virtual void EnsureIsEnabled();
    virtual bool IsVisible() const;
    //@}

    //! @name ReadOnly management // $$$$ ABR 2012-05-24: Factorize this to tools::ReadOnlyModable and use it on HierarchyListView_ABC
    //@{
    virtual bool IsReadOnly() const;
    void SetReadOnlyModes( int modes );
    //@}

private:
    //! @name Member data
    //@{
    GlWidget*     currentWidget_;
    MapLayer_ABC* currentProxy_;
    float         alpha_;
    std::string   passes_;
    bool          enabled_;
    int           readOnlyModes_;
    bool          readOnly_;
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
             Layer2D() {};
    virtual ~Layer2D() {};
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& ) {}
    virtual void RegisterIn( Gl3dWidget& )   {}
    virtual void UnregisterIn( Gl3dWidget& ) {}

    virtual void Reset2d();
    virtual void Reset();
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
             Layer3D() {}
    virtual ~Layer3D() {}
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& ) {}
    virtual void RegisterIn( GlWidget& )               {}
    virtual void UnregisterIn( GlWidget& )             {}

    virtual void Reset3d();
    virtual void Reset();
    //@}
};

}

#endif // __Layer_h_
