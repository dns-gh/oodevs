// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_ABC_h_
#define __Automat_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class AutomatType;
    class KnowledgeGroup_ABC;

// =============================================================================
/** @class  Automat_ABC
    @brief  Automat_ABC
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat_ABC : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat_ABC();
    virtual ~Automat_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const AutomatType& GetType() const = 0;
    virtual KnowledgeGroup_ABC& GetKnowledgeGroup() const = 0;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}
};

}

#endif // __Automat_ABC_h_
