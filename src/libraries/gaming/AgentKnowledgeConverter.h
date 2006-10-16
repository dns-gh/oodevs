// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledgeConverter_h_
#define __AgentKnowledgeConverter_h_

#include "AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class KnowledgeGroup_ABC;
}

// =============================================================================
/** @class  AgentKnowledgeConverter
    @brief  AgentKnowledgeConverter
*/
// Created: AGE 2006-05-18
// =============================================================================
class AgentKnowledgeConverter : public AgentKnowledgeConverter_ABC
                              , public kernel::Observer_ABC
                              , public kernel::ElementObserver_ABC< AgentKnowledge_ABC >
                              , public kernel::ElementObserver_ABC< PopulationKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgeConverter( kernel::Controllers& controllers );
    virtual ~AgentKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const AgentKnowledge_ABC*      Find( const AgentKnowledge_ABC& base, const kernel::Entity_ABC& owner );
    virtual const AgentKnowledge_ABC*      Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner );
    virtual const PopulationKnowledge_ABC* Find( const PopulationKnowledge_ABC& base, const kernel::Entity_ABC& owner );
    virtual const PopulationKnowledge_ABC* Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledgeConverter( const AgentKnowledgeConverter& );            //!< Copy constructor
    AgentKnowledgeConverter& operator=( const AgentKnowledgeConverter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const AgentKnowledge_ABC& );
    virtual void NotifyDeleted( const AgentKnowledge_ABC& );
    virtual void NotifyCreated( const PopulationKnowledge_ABC& );
    virtual void NotifyDeleted( const PopulationKnowledge_ABC& );

    const kernel::Entity_ABC* FindKnowledgeGroup( const kernel::Entity_ABC& owner ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Agent_ABC*, const AgentKnowledge_ABC* >           T_KnowledgeMap;
    typedef std::map< const kernel::Entity_ABC* , T_KnowledgeMap >                    T_Knowledges;
    typedef std::map< const kernel::Population_ABC*, const PopulationKnowledge_ABC* > T_PopulationKnowledgeMap;
    typedef std::map< const kernel::Entity_ABC* , T_PopulationKnowledgeMap >          T_PopulationKnowledges;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Knowledges           agents_;
    T_PopulationKnowledges populations_;
    //@}
};

#endif // __AgentKnowledgeConverter_h_
