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
#include "clients_kernel/AgentKnowledge_ABC.h"

// =============================================================================
/** @class  AgentKnowledgesLayer
    @brief  AgentKnowledgesLayer
*/
// Created: AGE 2006-05-17
// =============================================================================
class AgentKnowledgesLayer : public KnowledgeLayer< kernel::AgentKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledgesLayer( kernel::Controllers& controllers,
                                   gui::GLView_ABC& view,
                                   gui::ColorStrategy_ABC& strategy,
                                   const kernel::Profile_ABC& profile );
    virtual ~AgentKnowledgesLayer();
    //@}
};

#endif // __AgentKnowledgesLayer_h_
