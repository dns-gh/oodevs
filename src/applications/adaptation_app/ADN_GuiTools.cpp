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
#include "ADN_MultiRefWarningDialog.h"

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MultiRefWarning
// Created: APE 2005-01-27
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MultiRefWarning( ADN_Ref_ABC* data /* = 0 */ )
{
    ADN_Workspace::T_UsingElements elementsToDelete;
    ADN_Workspace::T_UsingElements usingElements;
    if( data )
    {
        elementsToDelete = ADN_Workspace::GetWorkspace().GetElementThatWillBeDeleted( data );
        usingElements = ADN_Workspace::GetWorkspace().GetElementThatUse( data );
    }
    if( elementsToDelete.empty() && usingElements.empty() )
        return QMessageBox::Ok == QMessageBox::warning( 0,
            qApp->translate( "ADN_Tools", "Multi references" ),
            qApp->translate( "ADN_Tools", "This item is referenced by at least one other item.\nClick \"Ok\" to destroy it and all its references." ),
            QMessageBox::Ok     | QMessageBox::Default,
            QMessageBox::Cancel | QMessageBox::Escape );

    return ADN_MultiRefWarningDialog( 0, elementsToDelete, usingElements ).exec() == QMessageBox::Ok;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::DeletionWarning
// Created: APE 2005-04-06
// -----------------------------------------------------------------------------
bool ADN_GuiTools::DeletionWarning()
{
    int nResult = QMessageBox::warning( 0,
        qApp->translate( "ADN_Tools", "Delete?" ),
        qApp->translate( "ADN_Tools", "Really delete this item?\nClick \"Ok\" to delete." ),
        QMessageBox::Ok     | QMessageBox::Default,
        QMessageBox::Cancel | QMessageBox::Escape );

    return nResult == QMessageBox::Ok;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MissingConvoyWarning
// Created: PHC 2011-01-19
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MissingConvoyWarning()
{
    QApplication::restoreOverrideCursor();
    QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        qApp->translate( "ADNDatabaseWarnings", "Convoy unit type not defined in Log/Supply, database cannot be saved." ),
        QMessageBox::Ok | QMessageBox::Default );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MissingConvoyMissionWarning
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MissingConvoyMissionWarning()
{
    QApplication::restoreOverrideCursor();
    QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        qApp->translate( "ADNDatabaseWarnings", "Convoy mission not defined in Log/Supply, database cannot be saved." ),
        QMessageBox::Ok | QMessageBox::Default );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MissingBreakdownWarning
// Created: PHC 2011-01-20
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MissingBreakdownWarning( const std::string& name )
{
    QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        tools::translate( "Composante_Data", "Equipment - Shall contain at least one breakdown for " )
        + QString::fromLatin1( name.c_str() )
        + QString::fromLatin1( ". \n" )
        + tools::translate( "Composante_Data", "Database cannot be saved." ),
        QMessageBox::Ok | QMessageBox::Default );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MissingPCOnAutomat
// Created: ABR 2012-07-04
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MissingPCOnAutomat( const std::string& name )
{
    QApplication::restoreOverrideCursor();
    QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        qApp->translate( "ADNDatabaseWarnings", "No PC defined for automat %1, database cannot be saved." ).arg( name.c_str() ),
        QMessageBox::Ok | QMessageBox::Default );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::BadAutomatComposition
// Created: ABR 2012-07-04
// -----------------------------------------------------------------------------
bool ADN_GuiTools::BadAutomatComposition( const std::string& name )
{
    QApplication::restoreOverrideCursor();
    QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        qApp->translate( "ADNDatabaseWarnings", "Automat %1 requires at least one PC, database cannot be saved." ).arg( name.c_str() ),
        QMessageBox::Ok | QMessageBox::Default );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MissingDecisionalModel
// Created: LGY 2012-09-21
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MissingDecisionalModel( const std::string& unit )
{
    QApplication::restoreOverrideCursor();
    QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        qApp->translate( "ADNDatabaseWarnings", "No decisional model defined for unit %1, database cannot be saved." ).arg( unit.c_str() ),
        QMessageBox::Ok | QMessageBox::Default );
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_GuiTools::MissingParameterChoices
// Created: LDC 2012-10-24
// -----------------------------------------------------------------------------
bool ADN_GuiTools::MissingParameterChoices( const std::string& name )
{
    QApplication::restoreOverrideCursor();
    QMessageBox::warning( 0,
        qApp->translate( "ADNDatabaseWarnings", "Database Work in progress" ),
        qApp->translate( "ADNDatabaseWarnings", "No parameter type specified for parameter %1, database cannot be saved." ).arg( name.c_str() ),
        QMessageBox::Ok | QMessageBox::Default );
    return false;
}
