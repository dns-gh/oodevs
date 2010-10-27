// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledgeNotifier_h_
#define __PopulationKnowledgeNotifier_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace MsgsSimToClient
{
    class MsgPopulationConcentrationKnowledgeUpdate;
}

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class PopulationKnowledge;
    class PopulationConcentrationKnowledge;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  PopulationKnowledgeNotifier
    @brief  PopulationKnowledgeNotifier
*/
// Created: DSO 2010-09-16
// =============================================================================
class PopulationKnowledgeNotifier : public kernel::Extension_ABC
//                        , public kernel::Updatable_ABC< ASN1T_MsgPopulationKnowledgeUpdate >
                            , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgeNotifier( kernel::Controller& controller, const dispatcher::PopulationKnowledge& knowledge );
    virtual ~PopulationKnowledgeNotifier();
    //@}

    //! @name Operations
    //@{
    //virtual void DoUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledgeNotifier( const PopulationKnowledgeNotifier& );            //!< Copy constructor
    PopulationKnowledgeNotifier& operator=( const PopulationKnowledgeNotifier& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::PopulationKnowledge& knowledge_;
    bool created_;
    //@}
};

}
}

#endif // __PopulationKnowledgeNotifier_h_
