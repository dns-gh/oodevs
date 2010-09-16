// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFire_h_
#define __PopulationFire_h_

#include "SimpleEntity.h"
#include "protocol/ClientSenders.h"

namespace kernel
{
    class ModelVisitor_ABC;
}
namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  PopulationFire
    @brief  PopulationFire
*/
// Created: AGE 2007-04-18
// =============================================================================
class PopulationFire : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFire( Model& model, const MsgsSimToClient::MsgStartPopulationFire& msg );
    virtual ~PopulationFire();
    //@}

    //! @name Operations
    //@{
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFire( const PopulationFire& );            //!< Copy constructor
    PopulationFire& operator=( const PopulationFire& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MsgsSimToClient::MsgStartPopulationFire msg_;
    //@}
};

}

#endif // __PopulationFire_h_
