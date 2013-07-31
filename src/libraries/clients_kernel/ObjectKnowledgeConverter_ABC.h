// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeConverter_ABC_h_
#define __ObjectKnowledgeConverter_ABC_h_

namespace kernel
{
    class Entity_ABC;
    class Object_ABC;
    class ObjectKnowledge_ABC;
    class Team_ABC;

// =============================================================================
/** @class  ObjectKnowledgeConverter_ABC
    @brief  ObjectKnowledgeConverter_ABC
*/
// Created: AGE 2006-09-15
// =============================================================================
class ObjectKnowledgeConverter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeConverter_ABC() {};
    virtual ~ObjectKnowledgeConverter_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long id, const kernel::Entity_ABC& owner ) const = 0;
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::ObjectKnowledge_ABC& base, const kernel::Entity_ABC& owner ) const = 0;
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& base, const kernel::Entity_ABC& owner ) const = 0;
    //@}
};

}

#endif // __ObjectKnowledgeConverter_ABC_h_
