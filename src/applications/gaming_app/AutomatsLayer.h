// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatsLayer_h_
#define __AutomatsLayer_h_

#include "clients_gui/AutomatsLayer.h"

// =============================================================================
/** @class  AutomatsLayer
    @brief  AutomatsLayer
*/
// Created: SBO 2007-04-13
// =============================================================================
class AutomatsLayer : public gui::AutomatsLayer
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, gui::AgentsLayer& agents );
    virtual ~AutomatsLayer();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatsLayer( const AutomatsLayer& );            //!< Copy constructor
    AutomatsLayer& operator=( const AutomatsLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    //@}
};

#endif // __AutomatsLayer_h_
