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

#include "clients_gui/KnowledgeLayer.h"
#include "gaming/PopulationKnowledge.h"

// =============================================================================
/** @class  PopulationKnowledgesLayer
    @brief  PopulationKnowledgesLayer
*/
// Created: AGE 2006-05-18
// =============================================================================
class PopulationKnowledgesLayer : public KnowledgeLayer< PopulationKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~PopulationKnowledgesLayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationKnowledgesLayer( const PopulationKnowledgesLayer& );            //!< Copy constructor
    PopulationKnowledgesLayer& operator=( const PopulationKnowledgesLayer& ); //!< Assignement operator
    //@}
};

#endif // __PopulationKnowledgesLayer_h_
