// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Logistics_h_
#define __Logistics_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Controller;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace sword
{
    class LogMaintenanceState;
    class LogMedicalState;
    class LogSupplyState;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  Logistics
    @brief  Logistics
*/
// Created: AGE 2006-03-01
// =============================================================================
class Logistics : public kernel::Extension_ABC
                , public kernel::Updatable_ABC< sword::LogMaintenanceState >
                , public kernel::Updatable_ABC< sword::LogMedicalState >
                , public kernel::Updatable_ABC< sword::LogSupplyState >
{
public:
    //! @name Constructors/Destructor
    //@{
             Logistics( kernel::Entity_ABC& holder, kernel::Controller& controller, Model& model, const StaticModel& staticModel, gui::PropertiesDictionary& dico );
    virtual ~Logistics();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Logistics( const Logistics& );            //!< Copy constructor
    Logistics& operator=( const Logistics& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::LogMedicalState& message );
    virtual void DoUpdate( const sword::LogMaintenanceState& message );
    virtual void DoUpdate( const sword::LogSupplyState& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& holder_;
    kernel::Controller& controller_;
    Model& model_;
    const StaticModel& static_;
    gui::PropertiesDictionary& dico_;
    //@}
};

#endif // __Logistics_h_
