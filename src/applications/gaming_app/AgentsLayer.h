// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsLayer_h_
#define __AgentsLayer_h_

#include "clients_gui/AgentsLayer.h"

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: SBO 2006-08-18
// =============================================================================
class AgentsLayer : public gui::AgentsLayer
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~AgentsLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DisplayTooltip( const kernel::Agent_ABC& entity, kernel::Displayer_ABC& displayer );
    template< typename Extension >
    void AddToTooltip( const kernel::Agent_ABC& entity, kernel::Displayer_ABC& displayer );
    //@}
};

#endif // __AgentsLayer_h_
