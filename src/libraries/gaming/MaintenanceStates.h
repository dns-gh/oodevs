// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MaintenanceStates_h_
#define __MaintenanceStates_h_

#include "Availability.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

using namespace Common;

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class EquipmentType;
    class Automat_ABC;
    class PropertiesDictionary;
}

namespace MsgsSimToClient
{
    class MsgLogMaintenanceState;
}
// =============================================================================
/** @class  MaintenanceStates
    @brief  MaintenanceStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class MaintenanceStates : public kernel::Extension_ABC
                        , public kernel::Updatable_ABC< MsgsSimToClient::MsgLogMaintenanceState >
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceStates( kernel::Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, kernel::PropertiesDictionary& dico );
    virtual ~MaintenanceStates();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const MsgsSimToClient::MsgLogMaintenanceState& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MaintenanceStates( const MaintenanceStates& );            //!< Copy constructor
    MaintenanceStates& operator=( const MaintenanceStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::EquipmentType* > T_Priorities;
    typedef std::vector< Availability >                 T_Availabilities;
    typedef std::vector< const kernel::Automat_ABC* >   T_Automats;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller&      controller_;
    const tools::Resolver_ABC< kernel::EquipmentType >& resolver_;
    const tools::Resolver_ABC< kernel::Automat_ABC >&   automatResolver_;

    bool             bChainEnabled_;
    unsigned         nWorkRate_;
    T_Priorities     priorities_;
    T_Automats       tacticalPriorities_;
    T_Availabilities dispoHaulers_;
    T_Availabilities dispoRepairers_;
    //@}
};

#endif // __MaintenanceStates_h_
