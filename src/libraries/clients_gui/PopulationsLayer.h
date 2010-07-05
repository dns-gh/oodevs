// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Gui_PopulationsLayer_h_
#define __Gui_PopulationsLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Population_ABC.h"

namespace gui
{

// =============================================================================
/** @class  PopulationsLayer
    @brief  PopulationsLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class PopulationsLayer : public EntityLayer< kernel::Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~PopulationsLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void ContextMenu( const kernel::Entity_ABC&, const geometry::Point2f&, const QPoint& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationsLayer( const PopulationsLayer& );            //!< Copy constructor
    PopulationsLayer& operator=( const PopulationsLayer& ); //!< Assignment operator
    //@}
};

}

#endif // __Gui_PopulationsLayer_h_
