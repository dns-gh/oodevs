// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup : public kernel::EntityImplementation< kernel::KnowledgeGroup_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( unsigned long nId, kernel::Controller& controller );
    virtual ~KnowledgeGroup();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeGroup( const KnowledgeGroup& );
    KnowledgeGroup& operator=( const KnowledgeGroup& );
    //@}
};

#endif // __KnowledgeGroup_h_
