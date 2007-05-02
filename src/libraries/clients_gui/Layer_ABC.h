// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Layer_ABC_h_
#define __Layer_ABC_h_

#include "graphics/MapLayer_ABC.h"

class ViewFrustum;

namespace kernel
{
    class Viewport_ABC;
}

namespace gui
{
    class GlWidget;
    class Gl3dWidget;

// =============================================================================
/** @class  Layer_ABC
    @brief  Layer_ABC
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer_ABC : public MapLayer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Layer_ABC();
    virtual ~Layer_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Paint( kernel::Viewport_ABC& viewport );

    virtual void RegisterIn( Gl3dWidget& widget );
    virtual void RegisterIn( GlWidget& widget );

    virtual void Reset2d();
    virtual void Reset3d();
    
    virtual void SetAlpha( float alpha );
    float GetAlpha() const;

    void MoveAbove( Layer_ABC& layer );
    void MoveBelow( Layer_ABC& layer );
    //@}
    
private:
    //! @name Member data
    //@{
    GlWidget*     currentWidget_;
    MapLayer_ABC* currentProxy_;
    float alpha_;
    //@}
};

// =============================================================================
/** @class  Layer2d_ABC
    @brief  Layer2d_ABC
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer2d_ABC : public Layer_ABC
{
 
public:
    //! @name Constructors/Destructor
    //@{
             Layer2d_ABC() {};
    virtual ~Layer2d_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& ) {}
    virtual void RegisterIn( Gl3dWidget& )   {}

    virtual void Reset2d();
    virtual void Reset();
    //@}
};

// =============================================================================
/** @class  Layer3d_ABC
    @brief  Layer3d_ABC
*/
// Created: AGE 2006-03-29
// =============================================================================
class Layer3d_ABC : public Layer_ABC
{
 
public:
    //! @name Constructors/Destructor
    //@{
             Layer3d_ABC() {}
    virtual ~Layer3d_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& ) {}
    virtual void RegisterIn( GlWidget& )               {}

    virtual void Reset3d();
    virtual void Reset();
    //@}
};

}

#endif // __Layer_ABC_h_
