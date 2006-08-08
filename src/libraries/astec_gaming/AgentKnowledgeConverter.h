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
#include "astec_kernel/ElementObserver_ABC.h"

class Controllers;

// =============================================================================
/** @class  AgentKnowledgeConverter
    @brief  AgentKnowledgeConverter
*/
// Created: AGE 2006-05-18
// =============================================================================
class AgentKnowledgeConverter : public AgentKnowledgeConverter_ABC
                              , public Observer_ABC
                              , public ElementObserver_ABC< AgentKnowledge >
                              , public ElementObserver_ABC< PopulationKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentKnowledgeConverter( Controllers& controllers );
    virtual ~AgentKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const AgentKnowledge*      Find( const AgentKnowledge& base, const KnowledgeGroup& owner );
    virtual const AgentKnowledge*      Find( const Agent_ABC& base,          const KnowledgeGroup& owner );
    virtual const PopulationKnowledge* Find( const PopulationKnowledge& base, const KnowledgeGroup& owner );
    virtual const PopulationKnowledge* Find( const Population_ABC& base,          const KnowledgeGroup& owner );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledgeConverter( const AgentKnowledgeConverter& );            //!< Copy constructor
    AgentKnowledgeConverter& operator=( const AgentKnowledgeConverter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const AgentKnowledge& );
    virtual void NotifyDeleted( const AgentKnowledge& );
    virtual void NotifyCreated( const PopulationKnowledge& );
    virtual void NotifyDeleted( const PopulationKnowledge& );
    //@}

    //! @name Types
    //@{
    typedef std::map< const Agent_ABC*, const AgentKnowledge* >                 T_KnowledgeMap;
    typedef std::map< const KnowledgeGroup* , T_KnowledgeMap >              T_Knowledges;
    typedef std::map< const Population_ABC*, const PopulationKnowledge* >       T_PopulationKnowledgeMap;
    typedef std::map< const KnowledgeGroup* , T_PopulationKnowledgeMap >    T_PopulationKnowledges;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    T_Knowledges           agents_;
    T_PopulationKnowledges populations_;
    //@}
};

#endif // __AgentKnowledgeConverter_h_
