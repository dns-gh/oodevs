// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyStates_h_
#define __SupplyStates_h_

#include "SupplyAvailability.h"
#include "Dotation.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Controller;
    class EquipmentType;
    class DotationType;
    class Displayer_ABC;
    class PropertiesDictionary;
}

namespace sword
{
    class LogSupplyState;
}

class SupplyAvailability;

// =============================================================================
/** @class  SupplyStates
    @brief  SupplyStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class SupplyStates : public kernel::Extension_ABC
                   , public kernel::Updatable_ABC< sword::LogSupplyState >
                   , public tools::Resolver< Dotation >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStates( kernel::Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, kernel::PropertiesDictionary& dico );
    virtual ~SupplyStates();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const sword::LogSupplyState& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyStates( const SupplyStates& );            //!< Copy constructor
    SupplyStates& operator=( const SupplyStates& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< SupplyAvailability >         T_Availabilities;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::EquipmentType >& resolver_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;

    bool             bChainEnabled_;
    T_Availabilities dispoTransporters_;
    //@}
};

#endif // __SupplyStates_h_
