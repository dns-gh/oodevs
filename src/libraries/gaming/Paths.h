// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Paths_h_
#define __Paths_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

namespace sword
{
    class UnitPathFind;
    class UnitAttributes;
    class UnitMagicAction;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  Paths
    @brief  Paths
*/
// Created: AGE 2006-02-13
// =============================================================================
class Paths : public kernel::Extension_ABC
            , public kernel::Updatable_ABC< sword::UnitPathFind >
            , public kernel::Updatable_ABC< sword::UnitAttributes >
            , public kernel::Updatable_ABC< sword::UnitMagicAction >
            , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Paths( const kernel::CoordinateConverter_ABC& converter );
    virtual ~Paths();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Paths( const Paths& );            //!< Copy constructor
    Paths& operator=( const Paths& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitPathFind& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void DoUpdate( const sword::UnitMagicAction& message );
    void UpdatePathfind();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    geometry::Rectangle2f plannedBox_;
    T_PointVector plannedPath_;
    geometry::Rectangle2f previousBox_;
    T_PointVector previousPath_;
    bool pendingMagicMove_;
    //@}
};

#endif // __Paths_h_
