// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_ABC_h_
#define __TacticalLine_ABC_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "clients_kernel/Types.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace gui
{
    class GlTools_ABC;
}

namespace tools
{
    class IdManager;
}

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  Base class of the limits and limas.
*/
// Created: APE 2004-04-14
// =============================================================================
class TacticalLine_ABC : public gui::EntityImplementation< kernel::TacticalLine_ABC >
                       , public kernel::Extension_ABC
                       , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC( kernel::Controller& controller, const QString& baseName, tools::IdManager& idManager );
             TacticalLine_ABC( kernel::Controller& controller, xml::xistream& xis, tools::IdManager& idManager );
    virtual ~TacticalLine_ABC();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );
    TacticalLine_ABC& operator=( const TacticalLine_ABC& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDestruction();
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}
};

#endif // __TacticalLine_ABC_h_
