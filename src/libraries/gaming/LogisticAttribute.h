// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "tools/Resolver_ABC.h"

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
