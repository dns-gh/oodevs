// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MaintenanceStates_h_
#define __MaintenanceStates_h_

#include "clients_kernel/Availability.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class EquipmentType;
    class Automat_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace sword
{
    class LogMaintenanceState;
}

// =============================================================================
/** @class  MaintenanceStates
    @brief  MaintenanceStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class MaintenanceStates : public kernel::Updatable_ABC< sword::LogMaintenanceState >
                        , public kernel::MaintenanceStates_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStates( kernel::Entity_ABC& entity, kernel::Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver,
                                const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, gui::PropertiesDictionary& dico );
    virtual ~MaintenanceStates();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const sword::LogMaintenanceState& message );
    virtual const std::vector< const kernel::EquipmentType* >& GetPriorities() const;
    virtual const std::vector< kernel::Availability >& GetDispoHaulers() const;
    virtual const std::vector< kernel::Availability >& GetDispoRepairers() const;
    virtual bool HasPriority( const kernel::EquipmentType* ) const;
    virtual bool IsEnabled() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MaintenanceStates( const MaintenanceStates& );            //!< Copy constructor
    MaintenanceStates& operator=( const MaintenanceStates& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dico ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::EquipmentType* > T_Priorities;
    typedef std::vector< kernel::Availability >         T_Availabilities;
    typedef std::vector< const kernel::Automat_ABC* >   T_Automats;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::EquipmentType >& resolver_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    bool bChainEnabled_;
    unsigned nWorkRate_;
    T_Priorities priorities_;
    T_Automats tacticalPriorities_;
    T_Availabilities dispoHaulers_;
    T_Availabilities dispoRepairers_;
    //@}
};

#endif // __MaintenanceStates_h_
