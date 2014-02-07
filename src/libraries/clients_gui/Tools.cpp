// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Tools.h"
#include "AutomatDecisions.h"

#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

#include <tools/Path.h>

bool tools::IsPropagationDir( const tools::Path& dir )
{
    return dir.IsDirectory() && ( dir / "propagation.xml" ).Exists();
}

bool tools::IsCommandPost( const kernel::Entity_ABC& entity )
{
    if( const kernel::CommandPostAttributes_ABC* pAttributes = entity.Retrieve< kernel::CommandPostAttributes_ABC >() )
        return pAttributes->IsCommandPost();
    return false;
}

bool tools::IsEngaged( const kernel::Entity_ABC& entity )
{
    if( const gui::AutomatDecisions* decisions =
        static_cast< const gui::AutomatDecisions* >( entity.Retrieve< gui::Decisions_ABC >() ) )
        return decisions->IsEngaged();
    return false;
}

bool tools::IsSuperiorEngaged( const kernel::Entity_ABC& entity )
{
    if( const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior() )
        return tools::IsEngaged( *superior );
    return false;
}

bool tools::LessThanById( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 )
{
    return entity1.GetId() < entity2.GetId();
}

bool tools::LessThanByPC( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 )
{
    if( IsCommandPost( entity1 ) )
        return true;
    if( IsCommandPost( entity2 ) )
        return false;
    return LessThanById( entity1, entity2 );
}

tools::Path tools::SanitizeFileName( QString text, const QString& after /*= "-"*/ )
{
    text.replace( QRegExp( "[/\"<>|*\?:\\\\]+" ), after );
    return tools::Path::FromUnicode( text.simplified().toStdWString() );
}
