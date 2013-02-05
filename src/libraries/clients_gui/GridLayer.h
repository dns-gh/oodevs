// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GridLayer_h_
#define __GridLayer_h_

#include "Layer.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class GlTools_ABC;
}

namespace gui
{
// =============================================================================
/** @class  GridLayer
    @brief  GridLayer
*/
// Created: AGE 2006-08-22
// =============================================================================
class GridLayer : public Layer2D
                , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GridLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools );
    virtual ~GridLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Paint( Viewport_ABC& viewport );
    //@}

private:
    //! @name Helpers
    //@{
    float Displace( float value );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::GlTools_ABC& tools_;

    geometry::Rectangle2f extent_;
    float gridSize_;
    //@}
};

}

#endif // __GridLayer_h_
