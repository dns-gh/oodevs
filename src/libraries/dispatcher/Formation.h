// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "game_asn/Simulation.h"
#include "SimpleEntity.h"
#include "clients_kernel/Formation_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Automat_ABC;
    class ModelVisitor_ABC;
    class Team_ABC;
}
namespace dispatcher
{
    class Model_ABC;
    class ClientPublisher_ABC;
    class Team_ABC;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Formation : public SimpleEntity< kernel::Formation_ABC > 
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( const Model_ABC& model, const ASN1T_MsgFormationCreation& msg );
    virtual ~Formation();
    //@}

    //! @name Operations
    //@{
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;

    virtual void Register( Formation_ABC& formation );
    virtual void Remove( Formation_ABC& formation );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC&                model_;
    const std::string           name_;

public:
    kernel::Team_ABC&           team_;
    const ASN1T_EnumNatureLevel level_;
    kernel::Formation_ABC*      parent_;
    tools::Resolver< kernel::Formation_ABC > formations_;
    tools::Resolver< kernel::Automat_ABC > automats_;
    //@}
};

}

#endif // __Formation_h_
