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

class KnowledgeGroup;

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
             KnowledgeGroupFactory_ABC() {};
    virtual ~KnowledgeGroupFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual KnowledgeGroup* CreateKnowledgeGroup( unsigned long id ) = 0; // $$$$ AGE 2006-02-15: team
    //@}
};

#endif // __KnowledgeGroupFactory_ABC_h_
