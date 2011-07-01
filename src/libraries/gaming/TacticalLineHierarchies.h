// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineHierarchies_h_
#define __TacticalLineHierarchies_h_

#include "clients_kernel/SimpleHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Formation_ABC;
}

// =============================================================================
/** @class  TacticalLineHierarchies
    @brief  TacticalLineHierarchies
*/
// Created: SBO 2006-11-14
// =============================================================================
class TacticalLineHierarchies : public kernel::SimpleHierarchies< kernel::TacticalHierarchies >
                              , public kernel::Updatable_ABC< sword::PhaseLineUpdate >
                              , public kernel::Updatable_ABC< sword::LimitUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
    TacticalLineHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const sword::Diffusion& message
                                    , const tools::Resolver_ABC< kernel::Automat_ABC >& automats, const tools::Resolver_ABC< kernel::Formation_ABC >& formations );
             TacticalLineHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const kernel::Entity_ABC& superior
                                    , const tools::Resolver_ABC< kernel::Automat_ABC >& automats, const tools::Resolver_ABC< kernel::Formation_ABC >& formations );
    virtual ~TacticalLineHierarchies();
    //@}

    //! @name Operations
    //@{
    void WriteTo( sword::Diffusion& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLineHierarchies( const TacticalLineHierarchies& );            //!< Copy constructor
    TacticalLineHierarchies& operator=( const TacticalLineHierarchies& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Update( const sword::Diffusion& message );
    virtual void DoUpdate( const sword::PhaseLineUpdate& message );
    virtual void DoUpdate( const sword::LimitUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automats_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formations_;
    bool superiorIsAutomat_;
    //@}
};

#endif // __TacticalLineHierarchies_h_
