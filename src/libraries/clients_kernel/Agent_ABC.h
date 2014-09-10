// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_ABC_h_
#define __Agent_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class AgentType;

// =============================================================================
/** @class  Agent_ABC
    @brief  Agent definition
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Agent_ABC();
    virtual ~Agent_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual const AgentType& GetType() const = 0;
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

#endif // __Agent_ABC_h_
