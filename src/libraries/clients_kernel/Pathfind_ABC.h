// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Pathfind_ABC_h_
#define __Pathfind_ABC_h_

#include "Entity_ABC.h"

namespace sword
{
    class PathResult;
    class Pathfind;
}

namespace kernel
{
// =============================================================================
/** @class  Pathfind_ABC
    @brief  Pathfind definition
*/
// =============================================================================
class Pathfind_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Pathfind_ABC();
    virtual ~Pathfind_ABC();
    //@}

    //! @name Methods
    //@{
    virtual const Entity_ABC& GetUnit() const = 0;
    virtual void SetVisible( bool visible ) = 0;
    virtual sword::PathResult GetPathfind() const = 0;
    virtual sword::Pathfind GetCreationMessage() const = 0;
    virtual geometry::Point2f GetPosition() const = 0;
    virtual void NotifyDestruction() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;
    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}
};

}

#endif // __Agent_ABC_h_
