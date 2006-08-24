// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationsLayerImp_h_
#define __PopulationsLayerImp_h_

#include "clients_gui/PopulationsLayer.h"

// =============================================================================
/** @class  PopulationsLayerImp
    @brief  PopulationsLayerImp
*/
// Created: SBO 2006-08-21
// =============================================================================
class PopulationsLayerImp : public gui::PopulationsLayer
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationsLayerImp( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view );
    virtual ~PopulationsLayerImp();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DisplayTooltip( const kernel::Population_ABC& entity, kernel::Displayer_ABC& displayer );
    //@}
};

#endif // __PopulationsLayerImp_h_
