// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_ObjectsLayer_h_
#define __gui_ObjectsLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Object_ABC.h"

namespace gui
{
    class TerrainPicker;

// =============================================================================
/** @class  ObjectsLayer
    @brief  ObjectsLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class ObjectsLayer : public EntityLayer< kernel::Object_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                           View_ABC& view, const kernel::Profile_ABC& profile, TerrainPicker& picker );
    virtual ~ObjectsLayer();
    //@}

    //! @name Operations
    //@{
    virtual QStringList TerrainPick( const geometry::Point2f& terrainCoordinates );
    //@}

protected:
    //! @name Helpers
    //@{
    //virtual bool IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual void Paint( kernel::Viewport_ABC& viewport );
    //@}
};

}

#endif // __gui_ObjectsLayer_h_
