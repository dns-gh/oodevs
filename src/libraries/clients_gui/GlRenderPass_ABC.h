// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlRenderPass_ABC_h_
#define __GlRenderPass_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    class GLView_ABC;

// =============================================================================
/** @class  GlRenderPass_ABC
    @brief  GlRenderPass_ABC
*/
// Created: SBO 2008-04-11
// =============================================================================
class GlRenderPass_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             GlRenderPass_ABC() {}
    virtual ~GlRenderPass_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const = 0;
    virtual void Render( GLView_ABC& view ) = 0;
    //@}
};

} //! namespace gui

#endif // __GlRenderPass_ABC_h_
