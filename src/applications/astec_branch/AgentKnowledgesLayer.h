// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgesLayer_h_
#define __AgentKnowledgesLayer_h_

#include "EntityLayer.h"
#include "AgentKnowledge.h"

// =============================================================================
/** @class  AgentKnowledgesLayer
    @brief  AgentKnowledgesLayer
*/
// Created: AGE 2006-05-17
// =============================================================================
class AgentKnowledgesLayer : public EntityLayer< AgentKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~AgentKnowledgesLayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledgesLayer( const AgentKnowledgesLayer& );            //!< Copy constructor
    AgentKnowledgesLayer& operator=( const AgentKnowledgesLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const Entity_ABC& );
    //@}
};

#endif // __AgentKnowledgesLayer_h_
