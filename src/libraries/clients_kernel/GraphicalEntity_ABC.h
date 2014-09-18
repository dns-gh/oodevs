// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __GraphicalEntity_ABC_h_
#define __GraphicalEntity_ABC_h_

#include <vector>

class QObject;
class QPoint;
class QString;
class QPixmap;

namespace kernel
{
    class ActionController;

// =============================================================================
/** @class  GraphicalEntity_ABC
    @brief  GraphicalEntity_ABC
*/
// Created: ABR 2013-02-04
// =============================================================================
class GraphicalEntity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GraphicalEntity_ABC() {}
    virtual ~GraphicalEntity_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const = 0;
    virtual QString GetTooltip() const = 0;
    //@}

    //! @name Mouse interactions
    //@{
    virtual void Select( ActionController& controller ) const = 0;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const GraphicalEntity_ABC* >& elements ) const = 0;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const = 0;
    virtual void Activate( ActionController& controller ) const = 0;
    virtual void OverFly( ActionController& controller ) const = 0;
    //@}

    //! @name Types
    //@{
    typedef std::vector< const GraphicalEntity_ABC* > T_GraphicalEntities;
    //@}
};

} //! namespace kernel

#endif // __GraphicalEntity_ABC_h_
