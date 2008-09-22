// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeNotifier_h_
#define __KnowledgeNotifier_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/simulation.h"

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class AgentKnowledge;
}

namespace script
{

// =============================================================================
/** @class  KnowledgeNotifier
    @brief  KnowledgeNotifier
*/
// Created: AGE 2008-07-16
// =============================================================================
class KnowledgeNotifier : public kernel::Extension_ABC
                        , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeNotifier( kernel::Controller& controller, const dispatcher::AgentKnowledge& knowledge );
    virtual ~KnowledgeNotifier();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeNotifier( const KnowledgeNotifier& );            //!< Copy constructor
    KnowledgeNotifier& operator=( const KnowledgeNotifier& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::AgentKnowledge& knowledge_;
    bool created_;
    //@}
};

}

#endif // __KnowledgeNotifier_h_
