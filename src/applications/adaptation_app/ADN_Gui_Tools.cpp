// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Gui_Tools.h"
#include "clients_gui/RichWidget.h"

QPushButton* ADN_Gui_Tools::CreatePushButton( const QString& objectName, const QString& text, const QIcon& icon, const QObject* receiver, const char* slot )
{
    QPushButton* button = new gui::RichWidget< QPushButton >( objectName );
    button->setText( text );
    button->setToolTip( text );
    button->setIcon( icon );
    button->setFixedWidth( 100 );
    QObject::connect( button, SIGNAL( clicked() ), receiver, slot );
    return button;
}
