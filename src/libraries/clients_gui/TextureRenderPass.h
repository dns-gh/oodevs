// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TextureRenderPass_h_
#define __TextureRenderPass_h_

#include "LayersRenderPass.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace gui
{
    class GLView_ABC;

// =============================================================================
/** @class  TextureRenderPass
    @brief  TextureRenderPass
*/
// Created: SBO 2008-04-14
// =============================================================================
class TextureRenderPass : public LayersRenderPass
{
public:
    //! @name Constructors/Destructor
    //@{
             TextureRenderPass( const std::string& name,
                                const std::string& option = "" );
    virtual ~TextureRenderPass();
    //@}

    //! @name Operations
    //@{
    virtual void Render( GLView_ABC& view );
    void BindTexture() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateTexture();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int texture_;
    std::string option_;
    bool enabled_;
    //@}
};

} //! namespace gui

#endif // __TextureRenderPass_h_
