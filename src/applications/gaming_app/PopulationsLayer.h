// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
             PopulationsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~PopulationsLayer();
    //@}
};

#endif // __PopulationsLayer_h_
