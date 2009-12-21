// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Formation.h"
#include "IdManager.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation::Formation( kernel::Controller& controller, const HierarchyLevel_ABC& level, IdManager& idManager )
    : EntityImplementation< Formation_ABC >( controller, idManager.GetNextId(), "" )
    , level_( &level )
{
    RegisterSelf( *this );
    name_ = tools::translate( "Formation", "Formation [%1]" ).arg( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Formation::Formation( xml::xistream& xis, Controller& controller, const FormationLevels& levels, IdManager& idManager )
    : EntityImplementation< Formation_ABC >( controller, 0, "" )
{
    std::string level, name;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "level", level )
        >> attribute( "name", name );
    level_ = levels.Resolve( level.c_str() );
    name_  = name.empty() ? tools::translate( "Formation", "Formation [%1]" ).arg( id_ ) : name.c_str();
    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Formation::GetName
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
QString Formation::GetName() const
{
    return level_->GetName() + ( name_.isEmpty() ? "" : " - " + name_ );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC& Formation::GetLevel() const
{
    return *level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Formation::Rename( const QString& name )
{
    const QString prefix = level_->GetName() + " - ";
    name_ = name.startsWith( prefix ) ? name.right( name.length() - prefix.length() ) : name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Formation::SerializeAttributes
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void Formation::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "name", name_.ascii() )
        << attribute( "level", level_->GetName().ascii() );
}

// -----------------------------------------------------------------------------
// Name: Formation::CreateDictionary
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void Formation::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/Name" ), name_ );
}

// -----------------------------------------------------------------------------
// Name: Formation::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Register( Formation_ABC& formation )
{
    throw std::exception( __FUNCTION__ " not implemented" ); 
}

// -----------------------------------------------------------------------------
// Name: Formation::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Remove( Formation_ABC& formation )
{
    throw std::exception( __FUNCTION__ " not implemented" ); 
}

// -----------------------------------------------------------------------------
// Name: Formation::Register
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Register( Automat_ABC& automat )
{
    throw std::exception( __FUNCTION__ " not implemented" ); 
}

// -----------------------------------------------------------------------------
// Name: Formation::Remove
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void Formation::Remove( Automat_ABC& automat )
{
    throw std::exception( __FUNCTION__ " not implemented" ); 
}

