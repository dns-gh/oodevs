// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPerceptions_h_
#define __ObjectPerceptions_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

using namespace Common;

namespace kernel
{
    class Automat_ABC;
    class Controller;
}

// =============================================================================
/** @class  ObjectPerceptions
    @brief  ObjectPerceptions
*/
// Created: AGE 2006-10-16
// =============================================================================
class ObjectPerceptions : public kernel::Extension_ABC
                        , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPerceptions( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& agentResolver );
    virtual ~ObjectPerceptions();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectPerceptions( const ObjectPerceptions& );            //!< Copy constructor
    ObjectPerceptions& operator=( const ObjectPerceptions& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Automat_ABC* > T_Agents;
    //@}

    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& agentResolver_;
public: // $$$$ AGE 2006-10-16: 
    T_Agents detectingAutomats_;
    //@}
};

#endif // __ObjectPerceptions_h_
