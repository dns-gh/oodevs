// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttributes_h_
#define __CrossingSiteAttributes_h_

#include "game_asn/Asn.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  CrossingSiteAttributes
    @brief  CrossingSiteAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class CrossingSiteAttributes : public kernel::CrossingSiteAttributes_ABC
                             , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                             , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                             , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit CrossingSiteAttributes( kernel::Controller& controller );
    virtual ~CrossingSiteAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossingSiteAttributes( const CrossingSiteAttributes& );            //!< Copy constructor
    CrossingSiteAttributes& operator=( const CrossingSiteAttributes& ); //!< Assignement operator
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
    kernel::OptionalValue< unsigned int > width_;
    kernel::OptionalValue< unsigned int > depth_;
    kernel::OptionalValue< unsigned int > speed_;
    kernel::OptionalValue< bool         > needsConstruction_;
    //@}
};

#endif // __CrossingSiteAttributes_h_
