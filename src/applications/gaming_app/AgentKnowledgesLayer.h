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

#include "KnowledgeLayer.h"
#include "gaming/AgentKnowledge.h"

// =============================================================================
/** @class  AgentKnowledgesLayer
    @brief  AgentKnowledgesLayer
*/
// Created: AGE 2006-05-17
// =============================================================================
class AgentKnowledgesLayer : public KnowledgeLayer< AgentKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgesLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view );
    virtual ~AgentKnowledgesLayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledgesLayer( const AgentKnowledgesLayer& );            //!< Copy constructor
    AgentKnowledgesLayer& operator=( const AgentKnowledgesLayer& ); //!< Assignement operator
    //@}
};

#endif // __AgentKnowledgesLayer_h_
