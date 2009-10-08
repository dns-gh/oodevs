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

namespace kernel
{
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  TextureRenderPass
    @brief  TextureRenderPass
*/
// Created: SBO 2008-04-14
// =============================================================================
class TextureRenderPass : public LayersRenderPass
                        , public tools::Observer_ABC
                        , public kernel::OptionsObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TextureRenderPass( MapWidget_ABC& widget, const std::string& name, kernel::Controllers& controllers, const std::string& option = "" );
    virtual ~TextureRenderPass();
    //@}

    //! @name Operations
    //@{
    virtual void Render( MapWidget_ABC& widget );
    void BindTexture() const;
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TextureRenderPass( const TextureRenderPass& );            //!< Copy constructor
    TextureRenderPass& operator=( const TextureRenderPass& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateTexture();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    unsigned int texture_;
    std::string option_;
    bool enabled_;
    //@}
};

}

#endif // __TextureRenderPass_h_
