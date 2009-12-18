// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Team_ABC_h_
#define __Team_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class Formation_ABC;
    class Karma;
    class KnowledgeGroup_ABC;
    class Object_ABC;
    class Population_ABC;

// =============================================================================
/** @class  Team_ABC
    @brief  Team base class
*/
// Created: SBO 2006-08-08
// =============================================================================
class Team_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Team_ABC();
    virtual ~Team_ABC();
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;
    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    virtual const kernel::Karma& GetKarma() const = 0;
    virtual void Register( Formation_ABC& formation ) = 0;
    virtual void Remove( Formation_ABC& formation ) = 0;
    virtual void Register( Population_ABC& population ) = 0;
    virtual void Remove( Population_ABC& population ) = 0;
    virtual void Register( Object_ABC& object ) = 0;
    virtual void Remove( Object_ABC& object ) = 0;
    virtual void Register( KnowledgeGroup_ABC& knGroup ) = 0;
    virtual void Remove( KnowledgeGroup_ABC& knGroup ) = 0;
    //@}
};

}

#endif // __Team_ABC_h_
