// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_ABC_h_
#define __Automat_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class AutomatType;

// =============================================================================
/** @class  Automat_ABC
    @brief  Automat_ABC
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Automat_ABC();
    virtual ~Automat_ABC();
    //@}

    //! @name Automat_ABC abstract method
    //@{
    virtual const AutomatType& GetType() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;
    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}
};

}

#endif // __Automat_ABC_h_
