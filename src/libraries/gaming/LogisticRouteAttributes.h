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

#include "ASN_Types.h"
#include "astec_kernel/Extension_ABC.h"
#include "astec_kernel/Updatable_ABC.h"
#include "astec_kernel/OptionalValue.h"
#include "astec_kernel/Resolver_ABC.h"

class Controller;
class Displayer_ABC;

// =============================================================================
/** @class  LogisticRouteAttributes
    @brief  LogisticRouteAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class LogisticRouteAttributes : public Extension_ABC
                              , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                              , public Updatable_ABC< ASN1T_MsgObjectUpdate >
                              , public Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticRouteAttributes( Controller& controller );
    virtual ~LogisticRouteAttributes();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
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
    Controller& controller_;
    OptionalValue< unsigned > nLogRouteFlow_;
    OptionalValue< unsigned > nLogRouteWidth_;
    OptionalValue< unsigned > nLogRouteLength_;
    OptionalValue< unsigned > nLogRouteMaxWeight_;
    OptionalValue< bool > bLogRouteEquipped_;
    //@}
};

#endif // __LogisticRouteAttributes_h_
