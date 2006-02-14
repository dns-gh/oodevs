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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "OptionalValue.h"
#include "Resolver_ABC.h"

class Controller;
class Object_ABC;

// =============================================================================
/** @class  LogisticRouteAttributes
    @brief  LogisticRouteAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class LogisticRouteAttributes : public Extension_ABC
                              , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticRouteAttributes();
    virtual ~LogisticRouteAttributes();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticRouteAttributes( const LogisticRouteAttributes& );            //!< Copy constructor
    LogisticRouteAttributes& operator=( const LogisticRouteAttributes& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    bool set_;

    unsigned nLogRouteFlow_;
    unsigned nLogRouteWidth_;
    unsigned nLogRouteLength_;
    unsigned nLogRouteMaxWeight_;
    bool bLogRouteEquipped_;
    //@}
};

#endif // __LogisticRouteAttributes_h_
