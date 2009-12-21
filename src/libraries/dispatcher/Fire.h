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

#include "game_asn/Simulation.h"
#include "SimpleEntity.h"

namespace kernel
{
    class ModelVisitor_ABC;
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
             Fire( Model& model, const ASN1T_MsgStartUnitFire& msg );
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
    ASN1T_UnitFire              oid_tir;
    ASN1T_Unit                  tireur;
    ASN1T_MsgStartUnitFire_type type;
    bool                        munitionPresent_;
    ASN1T_DotationType          munition;
    int                         typeCible_;
    ASN1T_OID                   oid_cible_;
    ASN1T_CoordLatLong          positionCible_;
    //@}
};

}

#endif // __Fire_h_
