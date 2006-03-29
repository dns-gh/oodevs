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

#include "EntityLayer.h"
#include "Agent.h"

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class AgentsLayer : public EntityLayer< Agent >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~AgentsLayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsLayer( const AgentsLayer& );            //!< Copy constructor
    AgentsLayer& operator=( const AgentsLayer& ); //!< Assignement operator
    //@}
};

#endif // __AgentsLayer_h_
