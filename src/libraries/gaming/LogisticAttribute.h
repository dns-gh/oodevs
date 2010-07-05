// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/OptionalValue.h"
#include "tools/Resolver_ABC.h"
#include "protocol/Protocol.h"

using namespace Common;

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Displayer_ABC;
}

// =============================================================================
/** @class  LogisticAttribute
    @brief  LogisticAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class LogisticAttribute : public kernel::LogisticAttribute_ABC
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectUpdate >
                     , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectCreation >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver );
    virtual ~LogisticAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticAttribute( const LogisticAttribute& );            //!< Copy constructor
    LogisticAttribute& operator=( const LogisticAttribute& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_NbcIds;
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
    const tools::Resolver_ABC< kernel::Automat_ABC >& resolver_;
    kernel::Automat_ABC* tc2_;
    //@}
};

#endif // __LogisticAttribute_h_
