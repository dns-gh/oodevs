

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_TOOLS_H
#define CLIENTS_GUI_TOOLS_H

#include <geometry/Types.h>

namespace kernel
{
    class Entity_ABC;
    class Profile_ABC;
}

namespace gui
{
    class GlTools_ABC;
}

namespace tools
{
    class Path;

    bool IsPropagationDir( const tools::Path& dir );
    bool IsCommandPost( const kernel::Entity_ABC& entity );
    bool IsEngaged( const kernel::Entity_ABC& entity );
    bool IsSuperiorEngaged( const kernel::Entity_ABC& entity );
    bool LessThanById( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 );
    bool LessThanByPC( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 );
    bool CanOneSubordinateBeOrdered( const kernel::Profile_ABC& profile, const kernel::Entity_ABC& entity );
    tools::Path SanitizeFileName( QString text, const QString& after = "-" );
    void SetKarma( const kernel::Entity_ABC& entity, std::string& symbol );
    void SetLevel( const kernel::Entity_ABC& entity, std::string& level );
    void DrawText( const QString& text, const QFont& font, const geometry::Point2f& point,
                   const QColor& color, const gui::GlTools_ABC& tools );

}  //! namespace tools

#endif  // CLIENTS_GUI_TOOLS_H
