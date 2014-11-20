// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LayersRenderPass_h_
#define __LayersRenderPass_h_

#include "GlRenderPass_ABC.h"

namespace gui
{

// =============================================================================
/** @class  LayersRenderPass
    @brief  LayersRenderPass
*/
// Created: SBO 2008-04-14
// =============================================================================
class LayersRenderPass : public GlRenderPass_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LayersRenderPass( const std::string& name, bool clear );
    virtual ~LayersRenderPass();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const;
    virtual void Render( GLView_ABC& view );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    bool clear_;
    //@}
};

} //! namespace gui

#endif // __LayersRenderPass_h_
