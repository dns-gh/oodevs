// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototype_h_
#define __ObjectPrototype_h_

#include "LocationSerializer.h"
#include "clients_gui/ObjectPrototype_ABC.h"

namespace kernel
{
    class Team_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
    namespace parameters
    {
        class ParameterList;
    }
}

class StaticModel;

// =============================================================================
/** @class  ObjectPrototype
    @brief  Object prototype
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype : public gui::ObjectPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, const kernel::Team_ABC& noSideTeam, gui::ParametersLayer& layer );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    void SetCommitContext( actions::ActionsModel& currentActionsModel, const kernel::Time_ABC& currentSimulationTime );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoCommit();
    //@}

    //! @name Copy/Assignment
    //@{
    ObjectPrototype( const ObjectPrototype& );            //!< Copy constructor
    ObjectPrototype& operator=( const ObjectPrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList* attributesList_;
    const StaticModel& static_;
    actions::ActionsModel* currentActionsModel_;
    const kernel::Time_ABC* currentSimulationTime_;
    //@}
};

#endif // __ObjectPrototype_h_
