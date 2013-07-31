// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ObjectKnowledgeConverter_h_
#define __ObjectKnowledgeConverter_h_

#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"

namespace dispatcher
{
    class Model_ABC;

// =============================================================================
/** @class  ObjectKnowledgeConverter
    @brief  ObjectKnowledgeConverter
*/
// Created: SBO 2010-05-11
// =============================================================================
class ObjectKnowledgeConverter : public kernel::ObjectKnowledgeConverter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectKnowledgeConverter( const dispatcher::Model_ABC& model );
    virtual ~ObjectKnowledgeConverter();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long id, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& base, const kernel::Entity_ABC& owner ) const;
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::ObjectKnowledge_ABC& base, const kernel::Entity_ABC& owner ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledgeConverter( const ObjectKnowledgeConverter& );            //!< Copy constructor
    ObjectKnowledgeConverter& operator=( const ObjectKnowledgeConverter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    //@}
};

}

#endif // __ObjectKnowledgeConverter_h_
