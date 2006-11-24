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
#include "clients_kernel/Resolver_ABC.h"
#include "ASN_Types.h"

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
                              , public kernel::Updatable_ABC< ASN1T_MsgLimaUpdate >
                              , public kernel::Updatable_ABC< ASN1T_MsgLimitUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLineHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const ASN1T_TacticalLinesDiffusion& asnMsg
                                    , const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, const kernel::Resolver_ABC< kernel::Formation_ABC >& formations );
             TacticalLineHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const kernel::Entity_ABC& superior
                                    , const kernel::Resolver_ABC< kernel::Automat_ABC >& automats, const kernel::Resolver_ABC< kernel::Formation_ABC >& formations );
    virtual ~TacticalLineHierarchies();
    //@}

    //! @name Operations
    //@{
    void WriteTo( ASN1T_TacticalLinesDiffusion& message ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalLineHierarchies( const TacticalLineHierarchies& );            //!< Copy constructor
    TacticalLineHierarchies& operator=( const TacticalLineHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Update( const ASN1T_TacticalLinesDiffusion& message );
    virtual void DoUpdate( const ASN1T_MsgLimaUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgLimitUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Automat_ABC >& automats_;
    const kernel::Resolver_ABC< kernel::Formation_ABC >& formations_;
    bool superiorIsAutomat_;
    //@}
};

#endif // __TacticalLineHierarchies_h_
