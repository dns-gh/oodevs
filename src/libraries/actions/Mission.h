// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionMission_h_
#define __ActionMission_h_

#include "ActionWithTarget_ABC.h"

namespace kernel
{
    class MissionType;
    class Controller;
    class GlTooltip_ABC;
}

namespace xml
{
    class xistream;
}

namespace actions
{

// =============================================================================
/** @class  Mission
    @brief  Mission
*/
// Created: SBO 2007-03-12
// =============================================================================
class Mission : public ActionWithTarget_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Mission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered = true );
             Mission( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity, bool stub );
    virtual ~Mission();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const ::gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Polish();
    unsigned int GetEntityId() const { return entityId_; }
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    bool registered_;
    mutable std::auto_ptr< kernel::GlTooltip_ABC > tooltip_;
    //@}
};

}

#endif // __ActionMission_h_
