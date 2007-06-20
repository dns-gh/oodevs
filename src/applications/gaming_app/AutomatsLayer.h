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

namespace kernel
{
    class CoordinateConverter_ABC;
    class AgentType;
}

class Publisher_ABC;

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
             AutomatsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, gui::AgentsLayer& agents, Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& converter );
    virtual ~AutomatsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
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
    virtual void NotifySelected( const kernel::Automat_ABC* automat );
    void RequestCreation( const geometry::Point2f& point, const kernel::AgentType& type );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    Publisher_ABC& publisher_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    //@}
};

#endif // __AutomatsLayer_h_
