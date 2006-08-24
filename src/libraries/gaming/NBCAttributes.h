// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCAttributes_h_
#define __NBCAttributes_h_

#include "ASN_Types.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class NBCAgent;
}

// =============================================================================
/** @class  NBCAttributes
    @brief  NBCAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class NBCAttributes : public kernel::NBCAttributes_ABC
                    , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                    , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                    , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             NBCAttributes( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::NBCAgent >& resolver );
    virtual ~NBCAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NBCAttributes( const NBCAttributes& );            //!< Copy constructor
    NBCAttributes& operator=( const NBCAttributes& ); //!< Assignement operator
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
    const kernel::Resolver_ABC< kernel::NBCAgent >& resolver_;
    const kernel::NBCAgent* nbc_;
    //@}
};

#endif // __NBCAttributes_h_
