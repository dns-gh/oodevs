// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_DRAWVISITOR_H
#define CLIENTS_GUI_DRAWVISITOR_H

#include "clients_gui/Drawer.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/ExtensionVisitor_ABC.h"

namespace gui
{
class DrawVisitor : public kernel::ExtensionVisitor_ABC< kernel::Extension_ABC >
{
public:
     DrawVisitor() {}
    ~DrawVisitor() {}

    virtual void Visit( const kernel::Extension_ABC& extension )
    {
        drawer.Register( extension );
    }

    void Draw( const geometry::Point2f& where,
               const Viewport_ABC& viewport,
               const kernel::GlTools_ABC& tools )
    {
        drawer.Draw( where, viewport, tools );
    }

private:
    Drawer drawer;
};
}

#endif // CLIENTS_GUI_DRAWVISITOR_H
