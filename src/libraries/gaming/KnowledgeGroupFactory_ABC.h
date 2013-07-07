// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroupFactory_ABC_h_
#define __KnowledgeGroupFactory_ABC_h_

namespace kernel
{
    class KnowledgeGroup_ABC;
}

namespace sword
{
    class KnowledgeGroupCreation;
}

// =============================================================================
/** @class  KnowledgeGroupFactory_ABC
    @brief  KnowledgeGroupFactory_ABC
*/
// Created: AGE 2006-02-15
// =============================================================================
class KnowledgeGroupFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupFactory_ABC() {}
    virtual ~KnowledgeGroupFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual kernel::KnowledgeGroup_ABC* CreateKnowledgeGroup( const sword::KnowledgeGroupCreation& message ) = 0; // LTO
    //@}
};

#endif // __KnowledgeGroupFactory_ABC_h_
