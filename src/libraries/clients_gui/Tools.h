

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

namespace kernel
{
    class Entity_ABC;
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
    tools::Path SanitizeFileName( QString text, const QString& after = "-" );
    void SetKarma( const kernel::Entity_ABC& entity, std::string& symbol );
    void SetLevel( const kernel::Entity_ABC& entity, std::string& level );

}  //! namespace tools

#endif  // CLIENTS_GUI_TOOLS_H
