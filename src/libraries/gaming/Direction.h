// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Direction_h_
#define __Direction_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class UnitKnowledgeUpdate;
    class UnitAttributes;
}

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  Direction
    @brief  Direction
*/
// Created: AGE 2007-12-17
// =============================================================================
class Direction : public kernel::Extension_ABC
                , public kernel::Updatable_ABC< sword::UnitAttributes >
                , public kernel::Updatable_ABC< sword::UnitKnowledgeUpdate >
                , public gui::Drawable_ABC
                , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Direction( const kernel::Entity_ABC& entity );
    virtual ~Direction();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& attributes );
    virtual void DoUpdate( const sword::UnitKnowledgeUpdate& attributes );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    geometry::Vector2f direction_;
    //@}
};

#endif // __Direction_h_
