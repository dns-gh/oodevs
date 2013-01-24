// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledgesLayer_h_
#define __PopulationKnowledgesLayer_h_

#include "KnowledgeLayer.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"

// =============================================================================
/** @class  PopulationKnowledgesLayer
    @brief  PopulationKnowledgesLayer
*/
// Created: AGE 2006-05-18
// =============================================================================
class PopulationKnowledgesLayer : public KnowledgeLayer< kernel::PopulationKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgesLayer( kernel::Controllers& controllers, const gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                                        gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~PopulationKnowledgesLayer();
    //@}
};

#endif // __PopulationKnowledgesLayer_h_
