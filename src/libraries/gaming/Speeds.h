// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Speeds_h_
#define __Speeds_h_

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
    class CoordinateConverter_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @class  Speeds
    @brief  Speeds
*/
// Created: AGE 2007-12-17
// =============================================================================
class Speeds : public kernel::Extension_ABC
             , public kernel::Updatable_ABC< sword::UnitAttributes >
             , public kernel::Updatable_ABC< sword::UnitKnowledgeUpdate >
             , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Speeds( const kernel::Entity_ABC& entity );
    virtual ~Speeds();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Speeds( const Speeds& );            //!< Copy constructor
    Speeds& operator=( const Speeds& ); //!< Assignment operator
    //@}

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
    float speed_;
    //@}
};

#endif // __Speeds_h_
