// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterDialogs.h"

#include "FilterDialog.h"
#include "FilterOrbatReIndexer.h"
#include "Menu.h"
#include "FilterCsv.h"
#include "clients_kernel/Tools.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: FilterDialogs constructor
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
FilterDialogs::FilterDialogs( QWidget* parent, const tools::ExerciseConfig& config, Model& model,
                              Menu& menu, const kernel::CoordinateConverter_ABC& converter )
    : parent_   ( parent )
    , config_   ( config )
    , model_    ( model )
    , menu_     ( menu )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterDialogs destructor
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
FilterDialogs::~FilterDialogs()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterDialogs::Load
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
void FilterDialogs::Load()
{
    assert( elements_.empty() );
    config_.GetLoader().LoadOptionalPhysicalFile( "filters", boost::bind( &FilterDialogs::Load, this, _1 ) );
    if( !Find( "import" ) )
        CreateImportDialog();
    if( !Find( "export" ) )
        CreateExportDialog();
    Get( "export" ).AddFilter( *new FilterCsv( &Get( "export" ), config_, model_, converter_ ) );
    Get( "import" ).AddFilter( *new FilterOrbatReIndexer( parent_, config_, model_ ) );
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->InsertMenuEntry( menu_ );
}

// -----------------------------------------------------------------------------
// Name: FilterDialogs::Load
// Created: ABR 2011-06-27
// -----------------------------------------------------------------------------
void FilterDialogs::Load( xml::xistream& xis )
{
    try
    {
        xis >> xml::start( "sections" )
                >> xml::list( "section", *this, &FilterDialogs::ReadSection )
            >> xml::end();
    }
    catch( std::exception& e )
    {
        QMessageBox::critical( parent_, tools::translate( "FilterDialogs", "Error loading filters" ), e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: FilterDialogs::Purge
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
void FilterDialogs::Purge()
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        menu_.RemoveFileMenuEntry( FilterDialog::menuIndex_ );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: FilterDialogs::ReadSection
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
void FilterDialogs::ReadSection( xml::xistream& xis )
{
    std::string name = xis.attribute< std::string >( "id" );
    std::transform( name.begin(), name.end(), name.begin(), std::tolower );
    if( name != "export" && name != "import" )
        throw std::runtime_error( tools::translate( "FilterDialogs", "Unknown section: %1." ).arg( name.c_str() ).ascii() );
    Register( name, *new FilterDialog( parent_, xis, config_ ) );
}

// -----------------------------------------------------------------------------
// Name: FilterDialogs::CreateImportDialog
// Created: ABR 2011-06-22
// -----------------------------------------------------------------------------
void FilterDialogs::CreateImportDialog()
{
    xml::xistringstream xis( "<sections><section id=\"import\"><descriptions><description xml:lang=\"en\" name=\"Import...\"/><description xml:lang=\"fr\" name=\"Importer...\"/></descriptions><filters></filters> </section></sections>" );
    Load( xis );
}

// -----------------------------------------------------------------------------
// Name: FilterDialogs::CreateExportDialog
// Created: LGY 2011-10-17
// -----------------------------------------------------------------------------
void FilterDialogs::CreateExportDialog()
{
    xml::xistringstream xis( "<sections><section id=\"export\"><descriptions><description xml:lang=\"en\" name=\"Export...\"/><description xml:lang=\"fr\" name=\"Exporter...\"/></descriptions><filters></filters> </section></sections>" );
    Load( xis );
}
