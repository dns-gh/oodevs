// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_h_
#define __TeamFactory_h_

#include "TeamFactory_ABC.h"

namespace kernel
{
    class Controllers;
    class PropertiesDictionary;
}

class Model;
class StaticModel;
class IdManager;
class ObjectAttributeFactory_ABC;

// =============================================================================
/** @class  TeamFactory
    @brief  TeamFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class TeamFactory : public TeamFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager );
    virtual ~TeamFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Team_ABC* CreateTeam();
    virtual kernel::Team_ABC* CreateTeam( xml::xistream& xis );
    virtual kernel::Team_ABC* CreateNoSideTeam();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& staticModel_;
    IdManager& idManager_;
    //@}
};

#endif // __TeamFactory_h_
