// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanKnowledgeConverter_ABC_h_
#define __UrbanKnowledgeConverter_ABC_h_

namespace kernel
{
    class Entity_ABC;
    class UrbanKnowledge_ABC;
    class Team_ABC;

// =============================================================================
/** @class  UrbanKnowledgeConverter_ABC
    @brief  UrbanKnowledgeConverter_ABC
*/
// Created: AGE 2006-09-15
// =============================================================================
class UrbanKnowledgeConverter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledgeConverter_ABC() {};
    virtual ~UrbanKnowledgeConverter_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual const kernel::UrbanKnowledge_ABC* Find( unsigned long id, const kernel::Team_ABC& owner ) const = 0;
    virtual const kernel::UrbanKnowledge_ABC* Find( unsigned long id, const kernel::Entity_ABC& owner ) const = 0;
    virtual const kernel::UrbanKnowledge_ABC* Find( const kernel::Entity_ABC& base, const kernel::Team_ABC& owner ) const = 0;
    //@}
};

}

#endif // __UrbanKnowledgeConverter_ABC_h_
