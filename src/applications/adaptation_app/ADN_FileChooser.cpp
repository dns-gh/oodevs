//*****************************************************************************
//
// $Created: JDY 03-07-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_FileChooser.cpp $
// $Author: Ape $
// $Modtime: 27/01/05 17:16 $
// $Revision: 3 $
// $Workfile: ADN_FileChooser.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_FileChooser.h"
#include "moc_ADN_FileChooser.cpp"
#include "ADN_Tools.h"

#include "clients_kernel/Tools.h"
#include "clients_gui/FileDialog.h"

#include <boost/algorithm/string.hpp>

QString ADN_FileChooser::szDefaultFilter_="All Files (*.*)";

class ADN_FileChooser_Connector : public ADN_Connector_ABC
{
public:

    typedef void( ADN_FileChooser::*T_OpSet )( const tools::Path& file );

    ADN_FileChooser_Connector( ADN_FileChooser& flc, T_OpSet op )
        : flc_( flc )
        , op_( op )
    {}

    virtual ~ADN_FileChooser_Connector()
    {}

    void SetDataPrivate( void *data )
    {
        assert( data );
        ( flc_.*op_ )( *reinterpret_cast< tools::Path* >( data ) );
    }

    void SetDataChanged( const tools::Path& p )
    {
        emit DataChanged( ( void* ) &p );
    }

private:
    ADN_FileChooser_Connector& operator=( const ADN_FileChooser_Connector& );

private:

    ADN_FileChooser& flc_;
    T_OpSet          op_;
};

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser constructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_FileChooser::ADN_FileChooser( QWidget *parent, const QString& filter, const char *name )
    : QWidget( parent,name )
    , eMode_( eFile )
    , szFilter_( filter )
    , szDirectory_( "" )
    , pButton_( 0 )
    , pLineEdit_( 0 )
    , vConnectors_( 2, static_cast< ADN_Connector_ABC* >( 0 ) )
{
    // objects
    Q3HBoxLayout *pLayout = new Q3HBoxLayout( this );
    pLayout->setMargin( 0 );

    pLineEdit_ = new QLineEdit( this );
    pLayout->addWidget( pLineEdit_ );

    pButton_ = new QPushButton( "...", this );
    pButton_->setFixedWidth( pButton_->fontMetrics().width( " ... " ) );
    pLayout->addWidget( pButton_ );

    setFocusProxy( pLineEdit_ );

    // connectors
    vConnectors_[ eFile ] = new ADN_FileChooser_Connector( *this, &ADN_FileChooser::SetFilename );
    vConnectors_[ eDirectory ] = new ADN_FileChooser_Connector( *this, &ADN_FileChooser::SetDirectory );

    connect( pLineEdit_, SIGNAL( textChanged( const QString & ) ), this, SLOT( FilenameChanged( const QString & ) ) );
    connect( pButton_, SIGNAL( clicked() ), this, SLOT( ChooseFile() ) );
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser destructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_FileChooser::~ADN_FileChooser()
{
    clear_owned_ptrs( vConnectors_ );
    vConnectors_.clear();
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::ChooseFile
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
void ADN_FileChooser::ChooseFile()
{
    tools::Path filename = eMode_ == eDirectory ? gui::FileDialog::getExistingDirectory( this, tools::translate( "ADN_FileChooser", "Choose a directory" ), szDirectory_ )
                                                : gui::FileDialog::getOpenFileName( this, tools::translate( "ADN_FileChooser", "Choose a file to open" ), szDirectory_, szFilter_ );

    if( filename.IsEmpty() )
        return;
    if( restrictions_.empty() )
        pLineEdit_->setText( QString::fromStdWString( filename.FileName().ToUnicode() ) );
    else
    {
        tools::Path file = filename;
        std::wstring filePath = file.Normalize().ToLower().ToUnicode();
        for( auto it = restrictions_.begin(); it != restrictions_.end(); ++it )
        {
            std::wstring lowerRestriction = *it;
            boost::algorithm::to_lower( lowerRestriction );
            std::size_t idx = filePath.find( lowerRestriction );
            if( idx != std::wstring::npos )
            {
                pLineEdit_->setText( QString::fromStdWString( filename.ToUnicode().substr( idx + lowerRestriction.size() +1 ) ) );
                return;
            }
        }
        QMessageBox::warning( this, tools::translate( "ADN_FileChooser", "Warning" ), tools::translate( "ADN_FileChooser", "This file is not in the correct file hierarchy and cannot be selected." ) );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::FilenameChanged
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_FileChooser::FilenameChanged( const QString& file )
{
    tools::Path p = tools::Path::FromUnicode( file.toStdWString() );
    static_cast< ADN_FileChooser_Connector* >( vConnectors_[ eFile ] )->SetDataChanged( p );
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::SetFileName
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
void ADN_FileChooser::SetFilename( const tools::Path &fn )
{
    int nPos = pLineEdit_->cursorPosition();
    pLineEdit_->setText( QString::fromStdWString( fn.ToUnicode() ) );
    pLineEdit_->setCursorPosition( nPos );
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::FileName
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
tools::Path ADN_FileChooser::GetFilename() const
{
    return tools::Path::FromUnicode( pLineEdit_->text().toStdWString() );
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::SetWorkingDir
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
void ADN_FileChooser::SetDirectory( const tools::Path& szDir )
{
    szDirectory_ = szDir;
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::SetMode
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
void ADN_FileChooser::SetMode( ADN_FileChooser::E_Mode m )
{
    eMode_ = m;
}

// -----------------------------------------------------------------------------
// Name: ADN_FileChooser::SetRestrictions
// Created: JSR 2013-05-23
// -----------------------------------------------------------------------------
void ADN_FileChooser::SetRestrictions( const std::vector< std::wstring >& restrictions )
{
    restrictions_ = restrictions;
}
