// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsModel_h_
#define __ActionsModel_h_

#include "tools/Resolver.h"

class Publisher_ABC;

namespace kernel
{
    class Entity_ABC;
    class MissionType;
    class FragOrderType;
}

namespace xml
{
    class xostream;
    class xistream;
}

namespace actions
{

class Action_ABC;
class ActionFactory_ABC;
class ActionsFilter_ABC;

// =============================================================================
/** @class  ActionsModel
    @brief  ActionsModel
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsModel : public tools::Resolver< Action_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionsModel( ActionFactory_ABC& factory, Publisher_ABC& publisher );
    virtual ~ActionsModel();
    //@}

    //! @name Operations
    //@{
    Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission );
    Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder );
    void Destroy( const Action_ABC& action );
    void Purge( const ActionsFilter_ABC* filter = 0 );
    void Load( const std::string& filename );
    void Save( const std::string& filename, const ActionsFilter_ABC* filter = 0 ) const;
    void Publish( const Action_ABC& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsModel( const ActionsModel& );            //!< Copy constructor
    ActionsModel& operator=( const ActionsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAction( xml::xistream& xis, std::string& errors );
    //@}

private:
    //! @name Member data
    //@{
    ActionFactory_ABC& factory_;
    Publisher_ABC& publisher_;
    //@}
};

}

#endif // __ActionsModel_h_
