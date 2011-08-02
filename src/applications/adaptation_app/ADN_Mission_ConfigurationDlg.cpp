// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Mission_ConfigurationDlg.cpp $
// $Author: Ape $
// $Modtime: 8/02/05 16:49 $
// $Revision: 2 $
// $Workfile: ADN_Mission_ConfigurationDlg.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Mission_ConfigurationDlg.h"

#include <Qt3Support/q3listview.h>
#include <QtGui/qlayout.h>
#include <Qt3Support/q3header.h>
#include <Qt3Support/q3hbox.h>
#include <QtGui/qpushbutton.h>

// -----------------------------------------------------------------------------
// Name: ADN_Mission_ConfigurationDlg constructor
/** @param  pParent
    @param  connector
*/
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_Mission_ConfigurationDlg::ADN_Mission_ConfigurationDlg( QWidget* pParent )
: QDialog( pParent, 0, true, Qt::WType_Dialog | Qt::WStyle_Customize | Qt::WStyle_NoBorder | Qt::WStyle_Title | Qt::WStyle_SysMenu | Qt::WType_Dialog )
, pListView_( 0 )
{
    setCaption( tr( "Mission configuration" ) );
    Q3VBoxLayout* pLayout = new Q3VBoxLayout( this );
    pLayout->setMargin( 5 );

    pListView_ = new Q3ListView( this );
    pListView_->addColumn( "missions" );
    pListView_->header()->hide();
    pLayout->addWidget( pListView_ );

    Q3HBox* pButtonBox = new Q3HBox( this );
    pButtonBox->setSpacing( 20 );
    pButtonBox->setMargin( 20 );

    QPushButton* pBtnOk = new QPushButton( tr( "OK" ), pButtonBox );
    QPushButton* pBtnCancel = new QPushButton( tr( "Cancel" ), pButtonBox );

    connect( pBtnOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( pBtnCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    pLayout->addWidget( pButtonBox );
}

// -----------------------------------------------------------------------------
// Name: ADN_Mission_ConfigurationDlg destructor
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
ADN_Mission_ConfigurationDlg::~ADN_Mission_ConfigurationDlg()
{

}
