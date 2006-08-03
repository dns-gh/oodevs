// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CampAttributes_h_
#define __CampAttributes_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "OptionalValue.h"
#include "Resolver_ABC.h"

class Controller;
class Agent_ABC;
class Displayer_ABC;

// =============================================================================
/** @class  CampAttributes
    @brief  CampAttributes
*/
// Created: AGE 2006-02-14
// =============================================================================
class CampAttributes : public Extension_ABC
                     , public Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                     , public Updatable_ABC< ASN1T_MsgObjectUpdate >
                     , public Updatable_ABC< ASN1T_MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             CampAttributes( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver );
    virtual ~CampAttributes();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CampAttributes( const CampAttributes& );            //!< Copy constructor
    CampAttributes& operator=( const CampAttributes& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_NbcIds;
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
    const Resolver_ABC< Agent_ABC >& resolver_;
    Agent_ABC* tc2_;
    //@}
};

#endif // __CampAttributes_h_
