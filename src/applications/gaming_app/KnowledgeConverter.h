// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeConverter_h_
#define __KnowledgeConverter_h_

#include "clients_kernel/KnowledgeConverter_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class KnowledgeGroup_ABC;
}

// =============================================================================
/** @class  KnowledgeConverter
    @brief  KnowledgeConverter
*/
// Created: AGE 2006-05-18
// =============================================================================
class KnowledgeConverter : public kernel::KnowledgeConverter_ABC
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< kernel::AgentKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit KnowledgeConverter( kernel::Controllers& controllers );
    virtual ~KnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentKnowledge_ABC* FindAgent( unsigned long id, const kernel::Entity_ABC& owner ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeConverter( const KnowledgeConverter& );            //!< Copy constructor
    KnowledgeConverter& operator=( const KnowledgeConverter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::AgentKnowledge_ABC& );
    virtual void NotifyDeleted( const kernel::AgentKnowledge_ABC& );

    const kernel::Entity_ABC* FindKnowledgeGroup( const kernel::Entity_ABC& owner ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const kernel::Agent_ABC*, const kernel::AgentKnowledge_ABC* > T_KnowledgeMap;
    typedef std::map< const kernel::Entity_ABC*, T_KnowledgeMap >                   T_Knowledges;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Knowledges agents_;
    //@}
};

#endif // __KnowledgeConverter_h_
