// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogButton.h"
#include "moc_LogButton.cpp"
#include "clients_kernel/Controllers.h"

namespace
{
    QPixmap GenerateDisabledPixmap( const QPixmap& pixmap )
    {
        QImage image = pixmap.toImage();
        for( auto i = 0; i < pixmap.width(); ++i )
            for( auto j = 0; j < pixmap.height(); ++j )
            {
                QRgb rgb = image.pixel( i, j );
                if( rgb != 0 )
                {
                    int gray = qGray( rgb );
                    image.setPixel( i, j, qRgb( gray, gray, gray ) );
                }
            }
        return pixmap.fromImage( image );
    }
}

// -----------------------------------------------------------------------------
// Name: LogButton constructor
// Created: LGY 2014-02-10
// -----------------------------------------------------------------------------
LogButton::LogButton( QWidget* parent, kernel::Controllers& controllers,
                      const QPixmap& pixmap )
    : QPushButton( parent )
    , controllers_   ( controllers )
    , enabledPixmap_ ( pixmap )
    , disabledPixmap_( GenerateDisabledPixmap( pixmap ) )
{
    setToggleButton( true );
    setPixmap( disabledPixmap_ );
    setFixedSize( 50, 50 );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogButton destructor
// Created: LGY 2014-02-10
// -----------------------------------------------------------------------------
LogButton::~LogButton()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogButton::ChangePixmap
// Created: LGY 2014-02-10
// -----------------------------------------------------------------------------
void LogButton::ChangePixmap( bool disable )
{
    setPixmap( disable ? disabledPixmap_ : enabledPixmap_ );
}

// -----------------------------------------------------------------------------
// Name: LogButton::NotifySelected
// Created: LGY 2014-02-10
// -----------------------------------------------------------------------------
void LogButton::NotifySelected( const kernel::Entity_ABC* element )
{
    setDisabled( element ? false : true );
}
