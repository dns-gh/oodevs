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
class PopulationsLayerImp : public PopulationsLayer
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationsLayerImp( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~PopulationsLayerImp();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DisplayTooltip( const Population_ABC& entity, Displayer_ABC& displayer );
    //@}
};

#endif // __PopulationsLayerImp_h_
