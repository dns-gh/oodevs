// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InputToxicCloudAttribute.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
InputToxicCloudAttribute::InputToxicCloudAttribute( kernel::PropertiesDictionary& dictionary, const QString& source,
                                                    const QString& dataField, bool bExport, const kernel::Entity_ABC& entity )
    : source_   ( source )
    , dataField_( dataField )
    , bExport_  ( bExport )
{
    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
InputToxicCloudAttribute::InputToxicCloudAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
    : bExport_( true )
{
    std::string source, dataField;
    xis >> xml::attribute( "source", source );
    xis >> xml::attribute( "data-field", dataField );
    xis >> xml::optional >> xml::attribute( "export", bExport_ );

    source_ = source.c_str();
    dataField_ = dataField.c_str();

    CreateDictionary( dictionary, entity );
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
InputToxicCloudAttribute::~InputToxicCloudAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::SerializeObjectAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "input-toxic-cloud" )
            << xml::attribute( "source", source_ )
            << xml::attribute( "data-field", dataField_ );
    if( ! bExport_ )
        xos << xml::attribute( "export", bExport_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity )
{
    dictionary.RegisterExtension( entity, this, tools::translate( "ToxicCloudAttribute", "Info/Cloud parameters/Data source" ), source_, true );
    dictionary.RegisterExtension( entity, this, tools::translate( "ToxicCloudAttribute", "Info/Cloud parameters/Data field" ), dataField_, true );
    dictionary.RegisterExtension( entity, this, tools::translate( "ToxicCloudAttribute", "Info/Cloud parameters/Data export" ), bExport_ );
}
