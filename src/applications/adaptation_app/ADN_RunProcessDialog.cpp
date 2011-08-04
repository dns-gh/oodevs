//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_RunProcessDialog.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 2 $
// $Workfile: ADN_RunProcessDialog.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_RunProcessDialog.h"
#include "moc_ADN_RunProcessDialog.cpp"

// -----------------------------------------------------------------------------
// Name: ADN_RunProcessDialog::ADN_RunProcessDialog
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
ADN_RunProcessDialog::ADN_RunProcessDialog( QWidget* pParent, const char* szDialogName )
: QDialog         ( pParent, szDialogName )
, pOutputField_   ( 0 )
, pProcess_       ( 0 )
{
    setCaption( szDialogName );
    Q3GridLayout* pGrid = new Q3GridLayout( this, 1, 1 );
    pOutputField_ = new Q3TextEdit( this );
    pOutputField_->setReadOnly( true );
    pOutputField_->setWordWrap( Q3TextEdit::NoWrap );
    pOutputField_->setMinimumSize( 500, 300 );
    pGrid->addWidget( pOutputField_, 0, 0 );
    pGrid->setColStretch( 0, 1 );
}


//-----------------------------------------------------------------------------
// Name: ADN_RunProcessDialog destructor
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
ADN_RunProcessDialog::~ADN_RunProcessDialog()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_RunProcessDialog::RunCommand
// Created: SBO 2006-01-25
// -----------------------------------------------------------------------------
void ADN_RunProcessDialog::RunCommand( const std::string& strCommandLine )
{
    pProcess_ = new Q3Process( QString( strCommandLine.c_str() ) );

    connect( pProcess_, SIGNAL( readyReadStdout() ), this, SLOT( ReadFromStdout() ) );
    connect( pProcess_, SIGNAL( readyReadStderr() ), this, SLOT( ReadFromStderr() ) );
    connect( pProcess_, SIGNAL( processExited() ), this, SLOT( ProcessFinished() ) );

    if( !pProcess_->start() )
        throw std::exception( "error starting process" );
    show();
}

// -----------------------------------------------------------------------------
// Name: ADN_RunProcessDialog::ReadFromStdout
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_RunProcessDialog::ReadFromStdout()
{
    QString str( pProcess_->readStdout() );
    if( str.length() > 0 )
        pOutputField_->append( str );
}

// -----------------------------------------------------------------------------
// Name: ADN_RunProcessDialog::ReadFromStderr
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_RunProcessDialog::ReadFromStderr()
{
    const QColor color = pOutputField_->color();
    pOutputField_->setColor( Qt::red );
    QString str( pProcess_->readStderr() );
    if( str.length() > 0 )
        pOutputField_->append( str );
    pOutputField_->setColor( color );
}

// -----------------------------------------------------------------------------
// Name: ADN_RunProcessDialog::ProcessFinished
// Created: SBO 2006-01-02
// -----------------------------------------------------------------------------
void ADN_RunProcessDialog::ProcessFinished()
{
    ReadFromStdout();
    if( pProcess_->exitStatus() == 0 )
    {
        delete pProcess_;
    }
}
