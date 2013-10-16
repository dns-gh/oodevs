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

#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include <tools/ElementObserver_ABC.h>

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
class AgentKnowledgeConverter : public kernel::AgentKnowledgeConverter_ABC
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< kernel::AgentKnowledge_ABC >
                              , public tools::ElementObserver_ABC< kernel::PopulationKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgeConverter( kernel::Controllers& controllers );
    virtual ~AgentKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentKnowledge_ABC*      FindAgent( unsigned long id, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::PopulationKnowledge_ABC* FindPopulation( unsigned long id, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::AgentKnowledge_ABC& base, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::PopulationKnowledge_ABC& base, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentKnowledgeConverter( const AgentKnowledgeConverter& );            //!< Copy constructor
    AgentKnowledgeConverter& operator=( const AgentKnowledgeConverter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::AgentKnowledge_ABC& );
    virtual void NotifyDeleted( const kernel::AgentKnowledge_ABC& );
    virtual void NotifyCreated( const kernel::PopulationKnowledge_ABC& );
    virtual void NotifyDeleted( const kernel::PopulationKnowledge_ABC& );

    const kernel::Entity_ABC* FindKnowledgeGroup( const kernel::Entity_ABC& owner ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const kernel::Agent_ABC*, const kernel::AgentKnowledge_ABC* >           T_KnowledgeMap;
    typedef std::map< const kernel::Entity_ABC* , T_KnowledgeMap >                            T_Knowledges;
    typedef std::map< const kernel::Population_ABC*, const kernel::PopulationKnowledge_ABC* > T_PopulationKnowledgeMap;
    typedef std::map< const kernel::Entity_ABC* , T_PopulationKnowledgeMap >                  T_PopulationKnowledges;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Knowledges agents_;
    T_PopulationKnowledges populations_;
    //@}
};

#endif // __AgentKnowledgeConverter_h_
