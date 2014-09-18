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

namespace kernel
{
    class Automat_ABC;

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
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup_ABC();
    virtual ~KnowledgeGroup_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;
    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const;
    virtual void Activate( ActionController& controller ) const;
    // LTO begin
    virtual bool IsActivated() const = 0;
    virtual bool IsCrowd() const = 0;
    //@}
};

}

#endif // __KnowledgeGroup_ABC_h_
