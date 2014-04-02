// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionMission_h_
#define __ActionMission_h_

#include "Action_ABC.h"

namespace kernel
{
    class MissionType;
    class Controller;
}

namespace gui
{
    class GlTooltip_ABC;
}

namespace actions
{

// =============================================================================
/** @class  Mission
    @brief  Mission
*/
// Created: SBO 2007-03-12
// =============================================================================
class Mission : public Action_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission( const kernel::MissionType* mission, kernel::Controller& controller, bool registered = true );
    virtual ~Mission();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const ::gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Polish();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    mutable std::auto_ptr< gui::GlTooltip_ABC > tooltip_;
    //@}
};

}

#endif // __ActionMission_h_
