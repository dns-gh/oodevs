// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentHierarchies_h_
#define __AgentHierarchies_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include "ASN_Types.h"

namespace kernel
{
    class Controller;
    class Automat_ABC;
}

// =============================================================================
/** @class  AgentHierarchies
    @brief  AgentHierarchies
*/
// Created: AGE 2006-09-20
// =============================================================================
class AgentHierarchies : public kernel::EntityHierarchies< kernel::CommunicationHierarchies >
                       , public kernel::Updatable_ABC< ASN1T_MsgPionCreation >
                       , public kernel::Updatable_ABC< ASN1T_MsgChangeAutomateAck >
                       , public kernel::Updatable_ABC< ASN1T_MsgChangeAutomate >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder,
                               const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver );
    virtual ~AgentHierarchies();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentHierarchies( const AgentHierarchies& );            //!< Copy constructor
    AgentHierarchies& operator=( const AgentHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPionCreation& message );
    virtual void DoUpdate( const ASN1T_MsgChangeAutomateAck& message );
    virtual void DoUpdate( const ASN1T_MsgChangeAutomate& message );

    void UpdateSuperior( kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver_; 
    //@}
};

#endif // __AgentHierarchies_h_
