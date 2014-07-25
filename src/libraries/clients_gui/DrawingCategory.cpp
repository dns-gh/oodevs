// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawingCategory.h"
#include "DrawingTemplate.h"
#include "clients_kernel/Controller.h"
#include <xeumeuleu/xml.hpp>
#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingCategory constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingCategory::DrawingCategory( xml::xistream& xis, svg::TextRenderer& renderer, kernel::Controller& controller,
                                  bool internalCategory )
    : controller_( controller )
    , name_( xis.attribute< std::string >( "name" ).c_str() )
    , id_( xis.attribute< std::string >( "id", "" ).c_str() )
    , description_( xis.content< std::string >( "description", "" ).c_str() )
    , internalCategory_( internalCategory )
{
    xis >> xml::list( "template", *this, &DrawingCategory::ReadTemplate, renderer );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawingCategory destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingCategory::~DrawingCategory()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: DrawingCategory::GetName
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
QString DrawingCategory::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DrawingCategory::GetDescription
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
QString DrawingCategory::GetDescription() const
{
    return description_;
}

// -----------------------------------------------------------------------------
// Name: DrawingCategory::ReadTemplate
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingCategory::ReadTemplate( xml::xistream& xis, svg::TextRenderer& renderer )
{
    DrawingTemplate* style = new DrawingTemplate( xis, name_, id_, renderer );

    if( !Find( style->GetName() ) && !Find( std::string( style->GetCode().toStdString() ) ) )
        Register( style->GetName(), style->GetCode().toStdString(), *style );
    else
        QMessageBox::critical( 0,   tools::translate( "gui::DrawingCategory", "Error" ),
                                    tools::translate( "gui::DrawingCategory", "'%1' : category already defined in drawings file." ).arg( style->GetName() ),
                                    QMessageBox::Ok, QMessageBox::NoButton );
}

// -----------------------------------------------------------------------------
// Name: DrawingCategory::GetTemplate
// Created: SBO 2010-05-20
// -----------------------------------------------------------------------------
const DrawingTemplate& DrawingCategory::GetTemplate( const std::string& templateName ) const
{
    const QString name = templateName.c_str();
    if( const DrawingTemplate* result = Find( name ) )
        return *result;
    return Get( templateName );
}

// -----------------------------------------------------------------------------
// Name: DrawingCategory::IsInternal
// Created: LGY 2014-07-23
// -----------------------------------------------------------------------------
bool DrawingCategory::IsInternal() const
{
    return internalCategory_;
}
