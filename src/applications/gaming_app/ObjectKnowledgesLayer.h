// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgesLayer_h_
#define __ObjectKnowledgesLayer_h_

#include "clients_gui/KnowledgeLayer.h"
#include "gaming/ObjectKnowledge.h"

// =============================================================================
/** @class  ObjectKnowledgesLayer
    @brief  ObjectKnowledgesLayer
*/
// Created: AGE 2006-05-18
// =============================================================================
class ObjectKnowledgesLayer : public KnowledgeLayer< ObjectKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgesLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~ObjectKnowledgesLayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgesLayer( const ObjectKnowledgesLayer& );            //!< Copy constructor
    ObjectKnowledgesLayer& operator=( const ObjectKnowledgesLayer& ); //!< Assignement operator
    //@}
};

#endif // __ObjectKnowledgesLayer_h_
