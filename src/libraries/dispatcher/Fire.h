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

namespace sword
{
    class FireId;
    class UnitId;
    class ResourceType;
    class CoordLatLong;
    class StartUnitFire;
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
             Fire( Model& model, const sword::StartUnitFire& msg );
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
    Fire& operator=( const Fire& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    sword::Id                          oid_tir_;
    sword::Id                          tireur_;
    sword::StartUnitFire::UnitFireType type_;
    bool                               munitionPresent_;
    sword::Id                          munition_;
    int                                typeCible_;
    int                                oid_cible_;
    int                                population_cible_;
    sword::CoordLatLong                positionCible_;
    //@}
};

}

#endif // __Fire_h_
