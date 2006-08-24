// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsLayerImp_h_
#define __AgentsLayerImp_h_

#include "clients_gui/AgentsLayer.h"

// =============================================================================
/** @class  AgentsLayerImp
    @brief  AgentsLayerImp
*/
// Created: SBO 2006-08-18
// =============================================================================
class AgentsLayerImp : public gui::AgentsLayer
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayerImp( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view );
    virtual ~AgentsLayerImp();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DisplayTooltip( const kernel::Agent_ABC& entity, kernel::Displayer_ABC& displayer );
    template< typename Extension >
    void AddToTooltip( const kernel::Agent_ABC& entity, kernel::Displayer_ABC& displayer );
    //@}
};

#endif // __AgentsLayerImp_h_
