// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_ABC_h_
#define __Formation_ABC_h_

#include "Entity_ABC.h"
#include "ENT/ENT_Enums.h"

namespace kernel
{

// =============================================================================
/** @class  Formation_ABC
    @brief  Formation_ABC
*/
// Created: SBO 2006-09-19
// =============================================================================
class Formation_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Formation_ABC();
    virtual ~Formation_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual E_NatureLevel GetLevel() const = 0;
    virtual QString GetBasicName() const;
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

#endif // __Formation_ABC_h_
