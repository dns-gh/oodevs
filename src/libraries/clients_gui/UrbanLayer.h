// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_UrbanLayer_h_
#define __gui_UrbanLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/UrbanObject_ABC.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
}

namespace gui
{
    class View_ABC;
}

namespace gui
{
// =============================================================================
/** @class  UrbanLayer
    @brief  UrbanLayer
*/
// Created: SLG 2006-03-23
// =============================================================================
class UrbanLayer : public EntityLayer< kernel::UrbanObject_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                         View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~UrbanLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( Viewport_ABC& viewport );
    virtual void Reset2d();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::UrbanObject_ABC& object );
    virtual void NotifySelectionChanged( const std::vector< const kernel::UrbanObject_ABC* >& elements );
    virtual void ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& geoPoint, const QPoint& point );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual void Draw( const kernel::Entity_ABC& entity, Viewport_ABC& viewport );
    virtual bool IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const;
    virtual bool IsInside( const kernel::Entity_ABC& entity, const geometry::Rectangle2f& rectangle ) const;
    virtual void ActivateEntity( const kernel::Entity_ABC& entity );
    virtual void Select( const kernel::Entity_ABC&, bool control, bool shift );
    //@}

private:
    //! @name Helpers
    //@{
    void DeselectAll();
    void DoSelect( const kernel::Entity_ABC* urbanObject );
    //@}

private:
    //! @name Member data
    //@{
    View_ABC& view_;
    //@}

protected:
    //! @name Member data
    //@{
    std::vector< const kernel::UrbanObject_ABC* > actualSelection_;
    kernel::Controllers& controllers_;
    //@}
};

}
#endif // __gui_UrbanLayer_h_
