// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligencesLayer_h_
#define __IntelligencesLayer_h_

#include "clients_gui/IntelligencesLayer.h"

// =============================================================================
/** @class  IntelligencesLayer
    @brief  IntelligencesLayer
*/
// Created: SBO 2007-10-15
// =============================================================================
class IntelligencesLayer : public gui::IntelligencesLayer
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligencesLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, kernel::IntelligenceFactory_ABC& factory );
    virtual ~IntelligencesLayer();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligencesLayer( const IntelligencesLayer& );            //!< Copy constructor
    IntelligencesLayer& operator=( const IntelligencesLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Delete( const kernel::Intelligence_ABC& element );
    virtual void Move( kernel::Positions& position, const geometry::Point2f& point );
    //@}
};

#endif // __IntelligencesLayer_h_
