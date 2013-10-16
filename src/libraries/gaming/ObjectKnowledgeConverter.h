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

#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>

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
class ObjectKnowledgeConverter : public kernel::ObjectKnowledgeConverter_ABC
                               , public tools::Observer_ABC
                               , public tools::ElementObserver_ABC< kernel::ObjectKnowledge_ABC >
                               , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectKnowledgeConverter( kernel::Controllers& controller );
    virtual ~ObjectKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long id, const kernel::KnowledgeGroup_ABC& owner ) const;
    virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long id, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::ObjectKnowledge_ABC& base, const kernel::KnowledgeGroup_ABC& owner ) const;
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& base, const kernel::KnowledgeGroup_ABC& owner ) const;
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& base, const kernel::Entity_ABC& owner ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::ObjectKnowledge_ABC& );
    virtual void NotifyDeleted( const kernel::ObjectKnowledge_ABC& );
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Object_ABC*, const kernel::ObjectKnowledge_ABC* > T_KnowledgeMap;
    typedef std::map< const kernel::Entity_ABC*, T_KnowledgeMap > T_Knowledges;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controller_;
    T_Knowledges knowledges_;
    //@}
};

#endif // __ObjectKnowledgeConverter_h_
