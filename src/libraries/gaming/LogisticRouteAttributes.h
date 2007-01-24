// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticRouteAttributes_h_
#define __LogisticRouteAttributes_h_

#include "network/Simulation_Asn.h"
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
/** @class  LogisticRouteAttributes
    @brief  LogisticRouteAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class LogisticRouteAttributes : public kernel::LogisticRouteAttributes_ABC
                              , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                              , public kernel::Updatable_ABC< ASN1T_MsgObjectUpdate >
                              , public kernel::Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticRouteAttributes( kernel::Controller& controller );
    virtual ~LogisticRouteAttributes();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticRouteAttributes( const LogisticRouteAttributes& );            //!< Copy constructor
    LogisticRouteAttributes& operator=( const LogisticRouteAttributes& ); //!< Assignement operator
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
    kernel::OptionalValue< unsigned > nLogRouteFlow_;
    kernel::OptionalValue< unsigned > nLogRouteWidth_;
    kernel::OptionalValue< unsigned > nLogRouteLength_;
    kernel::OptionalValue< unsigned > nLogRouteMaxWeight_;
    kernel::OptionalValue< bool > bLogRouteEquipped_;
    //@}
};

#endif // __LogisticRouteAttributes_h_
