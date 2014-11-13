// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Drawings_Data.h"
#include "ADN_Project_Data.h"
#include "svgl/TextRenderer.h"
#include "tools/Loader_ABC.h"

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data constructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Drawings_Data::ADN_Drawings_Data()
    : ADN_Data_ABC( eDrawings )
    , renderer_( new svg::TextRenderer() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data destructor
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
ADN_Drawings_Data::~ADN_Drawings_Data()
{
    for( auto it = geometryMap_.begin(); it != geometryMap_.end(); ++it )
        it->second.clear();
    geometryMap_.clear();
    for( auto it = categoryMap_.begin(); it != categoryMap_.end(); ++it )
        it->second.clear();
    categoryMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::FilesNeeded
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDrawingTemplates_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadArchive
// Created: ABR 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadArchive( xml::xistream& xis )
{
    xis >> xml::start( "templates" )
            >> xml::list( "category", *this, &ADN_Drawings_Data::ReadCategory );
    xml::xistringstream xss( "<template name=' - ' type='default'>"
                             "    <segment/>"
                             "</template>" );
    std::unique_ptr< DrawingInfo > drawing;
    drawing.reset( new DrawingInfo( xss >> xml::start( "template" ), "tasks", "tasks", *renderer_ ) );
    drawing->GenerateSamplePixmap();
    drawings_.AddItem( drawing.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadCategory
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadCategory( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name", "" );
    const std::string id = xis.attribute< std::string >( "id", "" );
    if( id != "" ) // $$$$ ABR 2011-04-22: check for hidden to display only tactical graphics category
        xis >> xml::list( "template", *this, &ADN_Drawings_Data::ReadTemplate, name, id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::ReadTemplate
// Created: SBO 2011-04-18
// -----------------------------------------------------------------------------
void ADN_Drawings_Data::ReadTemplate( xml::xistream& xis, const std::string& name, const std::string& id )
{
    try
    {
        std::unique_ptr< DrawingInfo > drawing;
        drawing.reset( new DrawingInfo( xis, name.c_str(), id.c_str(), *renderer_ ) );
        drawing->GenerateSamplePixmap();
        drawings_.AddItem( drawing.release() );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error reading drawing template " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetSymbol
// Created: ABR 2011-04-19
// -----------------------------------------------------------------------------
ADN_Drawings_Data::DrawingInfo* const ADN_Drawings_Data::GetDrawing( const std::string& code ) const
{
    for( auto it = drawings_.begin(); it != drawings_.end(); ++it )
        if( (*it)->GetCode() == code.c_str() )
            return *it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetGeometryDrawings
// Created: ABR 2011-04-21
// -----------------------------------------------------------------------------
ADN_Drawings_Data::T_DrawingInfoVector& ADN_Drawings_Data::GetGeometryDrawings( const std::string& geometries, const std::string& category )
{
    if( geometryMap_.find( geometries ) == geometryMap_.end() )
        geometryMap_[ geometries ] = ADN_Type_Vector_ABC< DrawingInfo >( true, false );
    T_DrawingInfoVector& currentVector = geometryMap_[ geometries ];
    if( currentVector.empty() )
    {
        QRegExp reg( "[, ]" );
        QStringList qlist = QStringList::split( reg, geometries.c_str() );

        for( auto it = drawings_.begin(); it != drawings_.end(); ++it )
            if( (*it)->GetId() == category.c_str() && qlist.contains( (*it)->GetType() ) )
                currentVector.AddItem( *it );
    }
    return currentVector;
}

// -----------------------------------------------------------------------------
// Name: ADN_Drawings_Data::GetCategoryDrawings
// Created: LGY 2011-08-30
// -----------------------------------------------------------------------------
ADN_Drawings_Data::T_DrawingInfoVector& ADN_Drawings_Data::GetCategoryDrawings( const std::string& category )
{
    if( categoryMap_.find( category ) == categoryMap_.end() )
        categoryMap_[ category ] = ADN_Type_Vector_ABC< DrawingInfo >( true, false );
    T_DrawingInfoVector& currentVector = categoryMap_[ category ];
    if( currentVector.empty() )
        for( auto it = drawings_.begin(); it != drawings_.end(); ++it )
            if( (*it)->GetId() == category.c_str() )
                currentVector.AddItem( *it );
    return currentVector;
}
