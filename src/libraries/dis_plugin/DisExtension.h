// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisExtension_h_
#define __DisExtension_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"
#include "hla_plugin/EntityIdentifier.h"

namespace dispatcher
{
    class Agent;
}

namespace dis
{
    class UdpNetwork;
    class Time_ABC;

// =============================================================================
/** @class  DisExtension
    @brief  DisExtension
*/
// Created: AGE 2008-03-10
// =============================================================================
class DisExtension : public kernel::Extension_ABC
                   , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             DisExtension( const Time_ABC& time, UdpNetwork& network, dispatcher::Agent& holder, const EntityIdentifier& id );
    virtual ~DisExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& attributes );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisExtension( const DisExtension& );            //!< Copy constructor
    DisExtension& operator=( const DisExtension& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const Time_ABC&    time_;
    UdpNetwork&        network_;
    dispatcher::Agent& holder_;
    EntityIdentifier   id_;
    //@}
};

}

#endif // __DisExtension_h_
