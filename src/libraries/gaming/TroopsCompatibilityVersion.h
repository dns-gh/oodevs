// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __TroopsCompatibilityVersion_h_
#define __TroopsCompatibilityVersion_h_

#include <boost/noncopyable.hpp>
#include "HierarchicExtension_ABC.h"
#include "Humans.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class Automat_ABC;
}

namespace sword
{
    class UnitAttributes;
}

// =============================================================================
/** @class  TroopsCompatibilityVersion
    @brief  TroopsCompatibilityVersion
*/
// Created: AGE 2006-02-13
// =============================================================================
class TroopsCompatibilityVersion : public kernel::Extension_ABC
                                 , public HierarchicExtension_ABC
                                 , public kernel::Updatable_ABC< sword::UnitAttributes >
                                 , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TroopsCompatibilityVersion( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~TroopsCompatibilityVersion();
    //@}

    //! @name Operators
    //@{
    int TroopsCompatibilityVersion::GetTotalHumans() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void SetSuperior( const kernel::Entity_ABC& superior );
    void Update( const Humans differences[ kernel::eTroopHealthStateNbrStates ] );
    void AddDifference( Humans differences[ kernel::eTroopHealthStateNbrStates ], kernel::E_TroopHealthState state, sword::EnumHumanRank rank, int value );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Humans              humans_[ kernel::eTroopHealthStateNbrStates ];
    //@}
};

#endif // __TroopsCompatibilityVersion_h_
