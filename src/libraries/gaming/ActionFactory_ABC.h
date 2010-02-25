// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionFactory_ABC_h_
#define __ActionFactory_ABC_h_

#include "actions/ActionFactory_ABC.h"

namespace Common
{
  class MsgUnitOrder;
  class MsgAutomatOrder;
  class MsgPopulationOrder;
}

// =============================================================================
/** @class  ActionFactory_ABC
    @brief  ActionFactory_ABC
*/
// Created: AGE 2008-07-16
// =============================================================================
class ActionFactory_ABC : public actions::ActionFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory_ABC() {};
    virtual ~ActionFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const = 0;
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const = 0;
    virtual actions::Action_ABC* CreateAction( xml::xistream& xis ) const = 0;

  virtual actions::Action_ABC* CreateAction( const Common::MsgUnitOrder& message ) const = 0;
    virtual actions::Action_ABC* CreateAction( const Common::MsgAutomatOrder& message ) const = 0;
    virtual actions::Action_ABC* CreateAction( const Common::MsgPopulationOrder& message ) const = 0;
    //@}
};


#endif // __ActionFactory_ABC_h_
