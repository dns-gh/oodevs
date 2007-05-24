// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeConverter_h_
#define __ObjectKnowledgeConverter_h_

#include "ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ObjectKnowledgeConverter
    @brief  ObjectKnowledgeConverter
*/
// Created: AGE 2006-09-15
// =============================================================================
class ObjectKnowledgeConverter : public ObjectKnowledgeConverter_ABC
                               , public kernel::Observer_ABC
                               , public kernel::ElementObserver_ABC< ObjectKnowledge_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectKnowledgeConverter( kernel::Controllers& controller );
    virtual ~ObjectKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const ObjectKnowledge_ABC* Find( unsigned long id, const kernel::Team_ABC& owner );
    virtual const ObjectKnowledge_ABC* Find( const ObjectKnowledge_ABC& base, const kernel::Team_ABC& owner );
    virtual const ObjectKnowledge_ABC* Find( const kernel::Object_ABC& base,  const kernel::Team_ABC& owner );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgeConverter( const ObjectKnowledgeConverter& );            //!< Copy constructor
    ObjectKnowledgeConverter& operator=( const ObjectKnowledgeConverter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const ObjectKnowledge_ABC& );
    virtual void NotifyDeleted( const ObjectKnowledge_ABC& );
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Object_ABC*, const ObjectKnowledge_ABC* >    T_KnowledgeMap;
    typedef std::map< const kernel::Team_ABC*, T_KnowledgeMap >                  T_Knowledges;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controller_;
    T_Knowledges knowledges_;
    //@}
};

#endif // __ObjectKnowledgeConverter_h_
