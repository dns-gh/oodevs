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
class AgentsLayerImp : public AgentsLayer
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayerImp( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~AgentsLayerImp();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DisplayTooltip( const Agent_ABC& entity, Displayer_ABC& displayer );
    template< typename Extension >
    void AddToTooltip( const Agent_ABC& entity, Displayer_ABC& displayer );
    //@}
};

#endif // __AgentsLayerImp_h_
