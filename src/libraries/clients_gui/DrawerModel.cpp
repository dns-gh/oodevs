// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawerModel.h"
#include "DrawerFactory.h"
#include "DrawerCategory.h"
#include "DrawerShape.h"
#include "clients_kernel/Controllers.h"
#include <boost/bind.hpp>
#include "xeumeuleu/xml.h"

using namespace gui;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: DrawerModel constructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerModel::DrawerModel(  kernel::Controllers& controllers, const DrawerFactory& factory )
    : controllers_( controllers )
    , factory_( factory )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel destructor
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
DrawerModel::~DrawerModel()
{
    controllers_.Unregister( *this );
    Purge();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Load
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerModel::Load( const std::string& filename )
{
    xml::xifstream input( filename );
    input >> xml::start( "templates" )
            >> xml::list( "category", *this, &DrawerModel::ReadCategory )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadCategory
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerModel::ReadCategory( xml::xistream& xis )
{
    std::auto_ptr< DrawerCategory > category( factory_.CreateCategory( xis ) );
    Register( category->GetName(), *category );
    category.release();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Purge
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerModel::Purge()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::LoadDrawings
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerModel::LoadDrawings( const std::string& filename )
{
    xml::xifstream xis( filename );
    xis >> start( "shapes" )
            >> list( "shape", *this, &DrawerModel::ReadShape )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ReadShape
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerModel::ReadShape( xml::xistream& xis )
{
    factory_.CreateShape( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::Save
// Created: SBO 2007-03-21
// -----------------------------------------------------------------------------
void DrawerModel::SaveDrawings( const std::string& filename ) const
{
    xml::xofstream xos( filename );
    xos << start( "shapes" );
    std::for_each( shapes_.begin(), shapes_.end(), boost::bind( &DrawerShape::Serialize, _1, boost::ref( xos ) ) );
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::ClearDrawings
// Created: SBO 2007-03-22
// -----------------------------------------------------------------------------
void DrawerModel::ClearDrawings()
{
    T_Shapes shapes( shapes_ );
    for( IT_Shapes it = shapes.begin(); it != shapes.end(); ++it )
        delete *it;
    shapes_.clear();
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::NotifyCreated
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
void DrawerModel::NotifyCreated( const DrawerShape& shape )
{
    shapes_.push_back( &shape );
}

// -----------------------------------------------------------------------------
// Name: DrawerModel::NotifyDeleted
// Created: AGE 2008-05-19
// -----------------------------------------------------------------------------
void DrawerModel::NotifyDeleted( const DrawerShape& shape )
{
    IT_Shapes it = std::find( shapes_.begin(), shapes_.end(), &shape );
    if( it != shapes_.end() )
    {
        std::swap( *it, shapes_.back() );
        shapes_.pop_back();
    }
}
