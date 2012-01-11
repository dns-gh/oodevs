// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EditorProxy_h_
#define __EditorProxy_h_

#include "clients_kernel/OptionsObserver_ABC.h"


namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Profile_ABC;
    class Entity_ABC;
    class OptionVariant;
}

namespace gui
{
    class ColorStrategy_ABC;
    class View_ABC;
    class LayerFilter_ABC;
    class TerrainPicker;
    class ParametersLayer;
}

// =============================================================================
/** @class  EditorProxy
    @brief  Editor proxy
*/
// Created: LGY 2012-01-03
// =============================================================================
template< typename T >
class EditorProxy : public T
                  , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EditorProxy( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
             EditorProxy( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::View_ABC& view, const kernel::Profile_ABC& profile, gui::TerrainPicker& picker, const gui::LayerFilter_ABC& filter );
             EditorProxy( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::ParametersLayer& parameters, gui::View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
    virtual ~EditorProxy();
    //@}

    //! @name Operations
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& entity );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

protected:
    //! @name Member data
    //@{
    bool livingAreaEditor_;
    //@}
};

#include "EditorProxy.inl"

#endif // __EditorProxy_h_
