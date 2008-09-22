// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_ABC_h_
#define __KnowledgeGroup_ABC_h_

#include "Entity_ABC.h"
#include "Resolver.h"

namespace kernel
{
    class Automat_ABC;
    class Team_ABC;

// =============================================================================
/** @class  KnowledgeGroup_ABC
    @brief  Knowledge group base class
*/
// Created: SBO 2006-08-09
// =============================================================================
class KnowledgeGroup_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup_ABC();
    virtual ~KnowledgeGroup_ABC();
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}
};

}

#endif // __KnowledgeGroup_ABC_h_
