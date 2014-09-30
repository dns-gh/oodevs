// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_ResourceNetworksLayer_h_
#define __gui_ResourceNetworksLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Entity_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class GlTools_ABC;
    class View_ABC;
}

namespace gui
{
// =============================================================================
/** @class  ResourceNetworksLayer
    @brief  Resource networks layer
*/
// Created: LGY 2013-05-15
// =============================================================================
class ResourceNetworksLayer : public EntityLayer< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworksLayer( kernel::Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                                    View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~ResourceNetworksLayer();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Entity_ABC& entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );

    virtual void Draw( const kernel::Entity_ABC& entity, Viewport_ABC& viewport, bool pickingMode );
    virtual bool IsInside( const kernel::Entity_ABC& entity, const geometry::Rectangle2f& rectangle ) const;
    virtual bool IsSelected( const kernel::Entity_ABC& entity ) const;
    virtual void ActivateEntity( const kernel::Entity_ABC& entity );
    //@}
};

}
#endif // __gui_ResourceNetworksLayer_h_
