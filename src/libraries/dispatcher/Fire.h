// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Fire_h_
#define __Fire_h_

#include "SimpleEntity.h"
#include "protocol/SimulationSenders.h"

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace Common
{
    class MsgUnitFire;
    class MsgUnit;
    enum MsgStartUnitFire_type;
    class MsgDotationType;
    class MsgCoordLatLong;
}

namespace MsgsSimToClient
{
    class MsgStartUnitFire;
}

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Fire
    @brief  Fire
*/
// Created: AGE 2007-04-18
// =============================================================================
class Fire : public SimpleEntity< >
{

public:
    //! @name Constructors/Destructor
    //@{
             Fire( Model& model, const MsgsSimToClient::MsgStartUnitFire& msg );
    virtual ~Fire();
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
    Fire( const Fire& );            //!< Copy constructor
    Fire& operator=( const Fire& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Common::MsgUnitFire              oid_tir;
    Common::MsgUnit                  tireur;
    Common::MsgStartUnitFire_type type;
    bool                        munitionPresent_;
    Common::MsgDotationType          munition;
    int                         typeCible_;
    /*OID*/ int                   oid_cible_;
    Common::MsgCoordLatLong          positionCible_;
    //@}
};

}

#endif // __Fire_h_
