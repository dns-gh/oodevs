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
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
InputToxicCloudAttribute::InputToxicCloudAttribute( kernel::PropertiesDictionary& dico )
    : bExport_( true )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
InputToxicCloudAttribute::InputToxicCloudAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico )
    : bExport_( true )
{
    std::string source, dataField;
    xis >> attribute( "source", source );
    xis >> attribute( "data-field", dataField );
    xis >> xml::optional() >> attribute( "export", bExport_ );
    
    source_ = source.c_str();
    dataField_ = dataField.c_str();

    CreateDictionary( dico );
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
// Name: InputToxicCloudAttribute::SetExportData
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::SetExportData( bool bExport )
{
    bExport_ = bExport;
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::SetSource
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::SetSource( const std::string& source, const std::string& dataField )
{
    source_ = source.c_str();
    dataField_ = dataField.c_str();
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::SerializeAttributes
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << start( "input-toxic-cloud" )            
            << attribute( "source", source_ )    
            << attribute( "data-field", dataField_ );
    if ( ! bExport_ )
        xos << attribute( "export", bExport_ ); 
    xos << end();
}

namespace
{
//    new QLabel( tools::translate( "InputPropagationPrototype_ABC", "Lookup data:" ), this );
//    dataField_ = new ValuedComboBox< std::string >( this );
//    // TODO : %TMP%
//    dataField_->AddItem( std::string( "Mesure C" ), std::string( "nom_var_shp_mesure_C" ) );
//    dataField_->AddItem( std::string( "Mesure Ct" ), std::string( "nom_var_shp_mesure_Ct" ) );
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::CreateDictionary
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
//    dico.Register( *this, tools::translate( "ToxicCloudAttribute", "Info/Cloud parameters" ), "Input cloud model" );
    dico.Register( *this, tools::translate( "ToxicCloudAttribute", "Info/Cloud parameters/Data source" ), source_ );
    dico.Register( *this, tools::translate( "ToxicCloudAttribute", "Info/Cloud parameters/Data field" ), dataField_ );
    dico.Register( *this, tools::translate( "ToxicCloudAttribute", "Info/Cloud parameters/Data export" ), bExport_ );
}
