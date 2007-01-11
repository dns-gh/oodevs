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
    using MapLayer_ABC::Paint;

    virtual void RegisterIn( Gl3dWidget& widget );
    virtual void RegisterIn( GlWidget& widget );
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
             Layer2d_ABC() : alpha_( 1 ) {};
    virtual ~Layer2d_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& ) {}
    virtual void RegisterIn( Gl3dWidget& ) {}

    void SetAlpha( float alpha ) { alpha_ = alpha; };
    float GetAlpha() const       { return alpha_; };
    //@}

private:
    //! @name Member data
    //@{
    float alpha_;
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
    virtual void RegisterIn( GlWidget& ) {}
    //@}
};

}

#endif // __Layer_ABC_h_
