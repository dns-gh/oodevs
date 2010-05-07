// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __actions_ActionFactory_ABC_h_
#define __actions_ActionFactory_ABC_h_

namespace Common
{
    class MsgUnitOrder;
    class MsgAutomatOrder;
    class MsgPopulationOrder;
}

namespace kernel
{
    class Entity_ABC;
    class MissionType;
    class FragOrderType;
}

namespace xml
{
    class xistream;
}

namespace actions
{

class Action_ABC;

// =============================================================================
/** @class  ActionFactory_ABC
    @brief  Action factory def
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory_ABC() {}
    virtual ~ActionFactory_ABC() {}
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

}

#endif // __actions_ActionFactory_ABC_h_
