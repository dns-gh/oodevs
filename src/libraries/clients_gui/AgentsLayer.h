// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_AgentsLayer_h_
#define __Gui_AgentsLayer_h_

#include "EntityLayer.h"

#include "clients_kernel/Agent_ABC.h"

namespace gui
{

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class AgentsLayer : public EntityLayer< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( kernel::Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                          View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~AgentsLayer();
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Select( const kernel::GraphicalEntity_ABC&, bool, bool );
    //@}
};

}

#endif // __Gui_AgentsLayer_h_
