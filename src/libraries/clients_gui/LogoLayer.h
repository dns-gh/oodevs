// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogoLayer_h_
#define __LogoLayer_h_

#include "Layer.h"

namespace gui
{
    class GlTools_ABC;
}

namespace gui
{

// =============================================================================
/** @class  LogoLayer
    @brief  LogoLayer
*/
// Created: AGE 2007-02-12
// =============================================================================
class LogoLayer : public Layer2D
{
public:
    //! @name Constructors/Destructor
    //@{
             LogoLayer( const gui::GlTools_ABC& tools, const QImage& image, float alpha = -1 );
    virtual ~LogoLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( Viewport_ABC& viewport );
    virtual void RegisterIn( Gl3dWidget& );
    virtual void SetAlpha( float alpha );
    //@}

private:
    //! @name Member data
    //@{
    const gui::GlTools_ABC& tools_;
    QImage image_;
    //@}
};

}

#endif // __LogoLayer_h_
