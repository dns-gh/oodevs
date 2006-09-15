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
    class Team_ABC;
    class Object_ABC;
}

class ObjectKnowledge;

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
    virtual const ObjectKnowledge* Find( const ObjectKnowledge& base,    const kernel::Team_ABC& owner ) = 0;
    virtual const ObjectKnowledge* Find( const kernel::Object_ABC& base, const kernel::Team_ABC& owner ) = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledgeConverter_ABC( const ObjectKnowledgeConverter_ABC& );            //!< Copy constructor
    ObjectKnowledgeConverter_ABC& operator=( const ObjectKnowledgeConverter_ABC& ); //!< Assignement operator
    //@}
};

#endif // __ObjectKnowledgeConverter_ABC_h_
