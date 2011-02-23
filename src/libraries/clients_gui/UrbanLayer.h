// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanLayer_h_
#define __UrbanLayer_h_

#include "Layer_ABC.h"
#include "EntityLayer.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/OptionVariant.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class View_ABC;

// =============================================================================
/** @class  UrbanLayer
@brief  UrbanLayer
*/
// Created: SLG 2006-03-23
// =============================================================================
class UrbanLayer : public EntityLayer< TerrainObjectProxy >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~UrbanLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( kernel::Viewport_ABC& viewport );
    virtual void Reset2d();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const TerrainObjectProxy& object );
    virtual void NotifySelected( const TerrainObjectProxy* object );
    virtual void ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& geoPoint, const QPoint& point );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    const TerrainObjectProxy*   selectedObject_;
    //@}
};

}
#endif // __UrbanLayer_h_
