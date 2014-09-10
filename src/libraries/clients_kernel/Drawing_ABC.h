// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __Drawing_ABC_h_
#define __Drawing_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
class Location_ABC;

// =============================================================================
/** @class  Drawing_ABC
    @brief  Drawing_ABC
*/
// Created: JSR 2012-06-06
// =============================================================================
class Drawing_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Drawing_ABC();
    virtual ~Drawing_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const GraphicalEntity_ABC* >& elements ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const;

    virtual const std::string& GetTypeName() const;
    virtual QColor GetColor() const = 0;
    virtual void Serialize( xml::xostream& xos ) const = 0;
    virtual const kernel::Entity_ABC* GetDiffusionEntity() const = 0;
    virtual void NotifyDestruction() const = 0;
    virtual const kernel::Location_ABC& GetLocation() const = 0;
    //@}
};

} // end namespace kernel

#endif // __Drawing_ABC_h_
