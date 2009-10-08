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
class Mission : public Action_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission( const kernel::Entity_ABC& entity, const kernel::MissionType& mission, kernel::Controller& controller, bool registered = true );
             Mission( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::MissionType >& missions, const kernel::Entity_ABC& entity );
    virtual ~Mission();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Polish();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Mission( const Mission& );            //!< Copy constructor
    Mission& operator=( const Mission& ); //!< Assignment operator
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
