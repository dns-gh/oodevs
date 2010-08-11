// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __FireAttribute_h_
#define __FireAttribute_h_


#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "tools/Resolver_ABC.h"
#include "protocol/Protocol.h"

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
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectCreation >
{
public:
    //! @name Constructors/Destructor
    //@{
             FireAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::FireClass >& resolver );
    virtual ~FireAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireAttribute( const FireAttribute& );            //!< Copy constructor
    FireAttribute& operator=( const FireAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectCreation& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::FireClass >& resolver_;
    const kernel::FireClass* fireClass_;
    int heat_;
    //@}
};

#endif // __FireAttribute_h_
