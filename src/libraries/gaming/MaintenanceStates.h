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

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "Availability.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class EquipmentType;
    class Agent_ABC;
    class DataDictionary;
}

// =============================================================================
/** @class  MaintenanceStates
    @brief  MaintenanceStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class MaintenanceStates : public kernel::Extension_ABC
                        , public kernel::Updatable_ABC< ASN1T_MsgLogMaintenanceEtat >
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStates( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::EquipmentType >& resolver, const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver, kernel::DataDictionary& dico );
    virtual ~MaintenanceStates();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const ASN1T_MsgLogMaintenanceEtat& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MaintenanceStates( const MaintenanceStates& );            //!< Copy constructor
    MaintenanceStates& operator=( const MaintenanceStates& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::EquipmentType* > T_Priorities;
    typedef std::vector< Availability >         T_Availabilities;
    typedef std::vector< const kernel::Agent_ABC* >         T_Agents;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller&      controller_;
    const kernel::Resolver_ABC< kernel::EquipmentType >& resolver_;
    const kernel::Resolver_ABC< kernel::Agent_ABC >& agentResolver_;

    bool             bChainEnabled_;
    unsigned         nWorkRate_;
    T_Priorities     priorities_;
    T_Agents         tacticalPriorities_;
    T_Availabilities dispoHaulers_;
    T_Availabilities dispoRepairers_;
    //@}
};

#endif // __MaintenanceStates_h_
