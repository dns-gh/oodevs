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

#include "EntityLayer.h"
#include "clients_kernel/Population_ABC.h"

// =============================================================================
/** @class  PopulationsLayer
    @brief  PopulationsLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class PopulationsLayer : public EntityLayer< Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view );
    virtual ~PopulationsLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DisplayTooltip( const Population_ABC& entity, Displayer_ABC& displayer );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationsLayer( const PopulationsLayer& );            //!< Copy constructor
    PopulationsLayer& operator=( const PopulationsLayer& ); //!< Assignement operator
    //@}
};

#endif // __PopulationsLayer_h_
