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
#include "Automat.h"
#include "SimpleEntity.h"
#include "clients_kernel/Formation_ABC.h"
#include "tools/Resolver.h"

namespace dispatcher
{
    class Model;
    class ModelVisitor_ABC;
    class ClientPublisher_ABC;
    class Side;
    class Automat;

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
             Formation( const Model& model, const ASN1T_MsgFormationCreation& msg );
    virtual ~Formation();
    //@}

    //! @name Operations
    //@{
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( ModelVisitor_ABC& visitor ) const;

    virtual const kernel::HierarchyLevel_ABC& GetLevel() const;
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
    const Model&                model_;
    const std::string           name_;

public:
    Side&           team_;
    const ASN1T_EnumNatureLevel level_;
    Formation*      parent_;
    tools::Resolver< Formation > formations_;
    tools::Resolver< Automat > automats_;
    //@}
};

}

#endif // __Formation_h_
