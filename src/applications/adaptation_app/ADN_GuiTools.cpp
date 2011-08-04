// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-09 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_GuiTools.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 10:37 $
// $Revision: 3 $
// $Workfile: ADN_GuiTools.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GuiTools.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MultiRefWarning
// Created: APE 2005-01-27
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MultiRefWarning()
{
    int nResult = QMessageBox::warning( 0,
        qApp->translate( "ADN_Tools", "Multi references" ),
        qApp->translate( "ADN_Tools", "This item is referenced by at least one other item.\nClick \"Ok\" to destroy it and all its references." ),
        QMessageBox::Ok     | QMessageBox::Default,
        QMessageBox::Cancel | QMessageBox::Escape );

    return nResult == QMessageBox::Ok;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::DeletionWarning
// Created: APE 2005-04-06
// -----------------------------------------------------------------------------
bool ADN_GuiTools::DeletionWarning()
{
    int nResult = QMessageBox::warning( 0,
        qApp->translate( "ADN_Tools", "Delete?" ),
        qApp->translate( "ADN_Tools", "Really delete this item?\nClick \"Ok\" to delete it and all its references." ),
        QMessageBox::Ok     | QMessageBox::Default,
        QMessageBox::Cancel | QMessageBox::Escape );

    return nResult == QMessageBox::Ok;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::WorkInProgressWarning
// Created: PHC 2011-01-19
// -----------------------------------------------------------------------------
bool ADN_GuiTools::WorkInProgressWarning()
{
    int nResult = QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress. \n Convoy unit type not defined. \n Do you want to save anyway ?" ),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No  | QMessageBox::Escape );
    return nResult == QMessageBox::Yes;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MissingBreakdownWarning
// Created: PHC 2011-01-20
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MissingBreakdownWarning( const std::string& name )
{
    int nResult = QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        tools::translate( "Composante_Data", "Equipment - Shall contain at least one breakdown for " )
      + QString::fromLatin1( name.c_str() )
      + QString::fromLatin1( ". \n Do you want to save anyway ?" ),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No  | QMessageBox::Escape );
    return nResult == QMessageBox::Yes;
}
