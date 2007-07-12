// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CompositeMessageHandler_h_
#define __CompositeMessageHandler_h_

#include "MessageHandler_ABC.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace dispatcher
{

// =============================================================================
/** @class  CompositeMessageHandler
    @brief  Composite message handler
*/
// Created: AGE 2007-07-09
// =============================================================================
class CompositeMessageHandler : public MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CompositeMessageHandler();
    virtual ~CompositeMessageHandler();
    //@}

    //! @name Operations
    //@{
    void Add( boost::shared_ptr< MessageHandler_ABC > handler );
    void Add( MessageHandler_ABC* handler );

    virtual void Receive( const ASN1T_MsgsSimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CompositeMessageHandler( const CompositeMessageHandler& );            //!< Copy constructor
    CompositeMessageHandler& operator=( const CompositeMessageHandler& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< MessageHandler_ABC > >   T_Handlers;
    typedef T_Handlers::const_iterator                             CIT_Handlers;
    //@}

private:
    //! @name Member data
    //@{
    T_Handlers handlers_;
    //@}
};

}

#endif // __CompositeMessageHandler_h_
