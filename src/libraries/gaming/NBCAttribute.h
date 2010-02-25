// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __NBCAttribute_h_
#define __NBCAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

using namespace Common;

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class NBCAgent;
}

// =============================================================================
/** @class  NBCAttribute
    @brief  NBCAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class NBCAttribute : public kernel::NBCAttribute_ABC
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectCreation >
{
public:
    //! @name Types
    //@{
    typedef std::vector< const kernel::NBCAgent* > T_NBCAgents;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             NBCAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::NBCAgent >& resolver );
    virtual ~NBCAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NBCAttribute( const NBCAttribute& );            //!< Copy constructor
    NBCAttribute& operator=( const NBCAttribute& ); //!< Assignement operator
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
    const tools::Resolver_ABC< kernel::NBCAgent >& resolver_;    
    T_NBCAgents agents_;
    int danger_;
    //@}
};

#endif // __NBCAttribute_h_
