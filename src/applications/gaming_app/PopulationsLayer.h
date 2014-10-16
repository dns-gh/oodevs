// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationsLayer_h_
#define __PopulationsLayer_h_

#include "clients_gui/PopulationsLayer.h"

// =============================================================================
/** @class  PopulationsLayer
    @brief  PopulationsLayer
*/
// Created: SBO 2006-08-21
// =============================================================================
class PopulationsLayer : public gui::PopulationsLayer
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationsLayer( kernel::Controllers& controllers,
                               gui::GLView_ABC& view,
                               gui::ColorStrategy_ABC& strategy,
                               const kernel::Profile_ABC& profile );
    virtual ~PopulationsLayer();
    //@}
};

#endif // __PopulationsLayer_h_
