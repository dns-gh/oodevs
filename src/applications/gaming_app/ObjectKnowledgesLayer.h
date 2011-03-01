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

#include "KnowledgeLayer.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"

// =============================================================================
/** @class  ObjectKnowledgesLayer
    @brief  ObjectKnowledgesLayer
*/
// Created: AGE 2006-05-18
// =============================================================================
class ObjectKnowledgesLayer : public KnowledgeLayer< kernel::ObjectKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgesLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                                    gui::View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
    virtual ~ObjectKnowledgesLayer();
    //@}
};

#endif // __ObjectKnowledgesLayer_h_
