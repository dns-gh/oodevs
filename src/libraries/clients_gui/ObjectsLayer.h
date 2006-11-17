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
             ObjectsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~ObjectsLayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectsLayer( const ObjectsLayer& );            //!< Copy constructor
    ObjectsLayer& operator=( const ObjectsLayer& ); //!< Assignement operator
    //@}
};

}

#endif // __gui_ObjectsLayer_h_
