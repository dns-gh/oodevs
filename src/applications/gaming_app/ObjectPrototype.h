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

namespace actions
{
    class ActionsModel;
    namespace parameters
    {
        class ParameterList;
    }
}

class ActionPublisher;
class Simulation;
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
             ObjectPrototype( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, gui::ParametersLayer& layer );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    void Commit( ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const Simulation& simulation );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPrototype( const ObjectPrototype& );            //!< Copy constructor
    ObjectPrototype& operator=( const ObjectPrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    actions::parameters::ParameterList* attributesList_;
    const StaticModel& static_;
    //@}
};

#endif // __ObjectPrototype_h_
