// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireAttribute_h_
#define __FireAttribute_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class FireClass;
}

// =============================================================================
/** @class  FireAttribute
    @brief  FireAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class FireAttribute : public kernel::FireAttribute_ABC
                    , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                    , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                    , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             FireAttribute( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::FireClass >& resolver );
    virtual ~FireAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FireAttribute( const FireAttribute& );            //!< Copy constructor
    FireAttribute& operator=( const FireAttribute& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::FireClass >& resolver_;
    const kernel::FireClass* fireClass_;
    int heat_;
    //@}
};

#endif // __FireAttribute_h_
