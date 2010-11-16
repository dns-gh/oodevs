// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanKnowledgeConverter_h_
#define __UrbanKnowledgeConverter_h_

#include "clients_kernel/UrbanKnowledgeConverter_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  UrbanKnowledgeConverter
    @brief  UrbanKnowledgeConverter
*/
// Created: AGE 2006-09-15
// =============================================================================
class UrbanKnowledgeConverter : public kernel::UrbanKnowledgeConverter_ABC
                               , public tools::Observer_ABC
                               , public tools::ElementObserver_ABC< kernel::UrbanKnowledge_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanKnowledgeConverter( kernel::Controllers& controller );
    virtual ~UrbanKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::UrbanKnowledge_ABC* Find( unsigned long id, const kernel::Team_ABC& owner ) const;
    virtual const kernel::UrbanKnowledge_ABC* Find( unsigned long id, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::UrbanKnowledge_ABC* Find( const kernel::Entity_ABC& base, const kernel::Team_ABC& owner ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanKnowledgeConverter( const UrbanKnowledgeConverter& );            //!< Copy constructor
    UrbanKnowledgeConverter& operator=( const UrbanKnowledgeConverter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::UrbanKnowledge_ABC& );
    virtual void NotifyDeleted( const kernel::UrbanKnowledge_ABC& );
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Entity_ABC*, const kernel::UrbanKnowledge_ABC* > T_KnowledgeMap;
    typedef T_KnowledgeMap::const_iterator                             CIT_KnowledgeMap;
    typedef std::map< const kernel::Entity_ABC*, T_KnowledgeMap >          T_Knowledges;
    typedef T_Knowledges::const_iterator                                 CIT_Knowledges;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controller_;
    T_Knowledges knowledges_;
    //@}
};

#endif // __UrbanKnowledgeConverter_h_
