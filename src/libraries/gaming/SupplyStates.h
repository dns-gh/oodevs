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

#include "Dotation.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class EquipmentType;
    class DotationType;
    class Displayer_ABC;
    class Availability;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace sword
{
    class LogSupplyState;
}

// =============================================================================
/** @class  SupplyStates
    @brief  SupplyStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class SupplyStates : public kernel::Extension_ABC
                   , public kernel::Updatable_ABC< sword::LogSupplyState >
                   , public tools::Resolver< Dotation >
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStates( kernel::Entity_ABC& entity, kernel::Controller& controller,
                           const tools::Resolver_ABC< kernel::EquipmentType >& resolver,
                           const tools::Resolver_ABC< kernel::DotationType >& dotationResolver,
                           gui::PropertiesDictionary& dico );
    virtual ~SupplyStates();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const sword::LogSupplyState& message );
    Dotation* GetConnectedNetworkStock() const;
    //@}

    //! @name Accessors
    //@{
    bool IsChainEnabled() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dico ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< kernel::Availability >         T_Availabilities;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Entity_ABC&                                 entity_;
    kernel::Controller&                                 controller_;
    const tools::Resolver_ABC< kernel::EquipmentType >& resolver_;
    const tools::Resolver_ABC< kernel::DotationType >&  dotationResolver_;
    std::unique_ptr< Dotation >                         resourceNetworkStock_;
    bool                                                bChainEnabled_;
    T_Availabilities                                    dispoTransporters_;
    const QString                                       property_;
    //@}
};

#endif // __SupplyStates_h_
