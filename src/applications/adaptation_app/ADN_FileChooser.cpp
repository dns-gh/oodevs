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

#include <QtGui/qlayout.h>
#include <QtGui/qlineedit.h>
#include <Qt3Support/q3filedialog.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qmessagebox.h>

QString ADN_FileChooser::szDefaultFilter_="All Files (*.*)";



class ADN_FileChooser_Connector
: public ADN_Connector_ABC
{
public:

    typedef void (ADN_FileChooser::*T_OpSet)(const QString& file);

    ADN_FileChooser_Connector(ADN_FileChooser& flc,T_OpSet op): ADN_Connector_ABC() , flc_(flc), op_(op)
    {}

    virtual ~ADN_FileChooser_Connector()
    {}

    void SetDataPrivate(void *data)
    {
        assert(data);
        (flc_.*op_)(((std::string*)data)->c_str());
    }

    void  SetDataChanged(const QString& string)
    {
        std::string newval=string.ascii();
        emit DataChanged((void*)&newval);
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
ADN_FileChooser::ADN_FileChooser(QWidget *parent,const QString& filter,const char *name)
:   QWidget(parent,name)
,   eMode_(eFile)
,   szFilter_(filter)
,   szDirectory_("")
,   pButton_(0)
,   pLineEdit_(0)
,   vConnectors_(2,(ADN_Connector_ABC*)0 )
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
    vConnectors_[eFile]     =new ADN_FileChooser_Connector(*this,&ADN_FileChooser::SetFilename);
    vConnectors_[eDirectory]=new ADN_FileChooser_Connector(*this,&ADN_FileChooser::SetDirectory);



    connect( pLineEdit_             , SIGNAL( textChanged( const QString & ) ),
             this                   , SLOT( FilenameChanged( const QString & ) ) );

    connect( pButton_   , SIGNAL( clicked() ),
             this       , SLOT( ChooseFile() ) );

}


//-----------------------------------------------------------------------------
// Name: ADN_FileChooser destructor
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
ADN_FileChooser::~ADN_FileChooser()
{
    clear_owned_ptrs(vConnectors_);
    vConnectors_.clear();
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
inline
std::string GetPartPath(const std::string szWorking,const std::string& full)
{
    if (_strcmpi(szWorking.c_str(),full.substr(0,szWorking.size()).c_str()))
        return std::string();
    else
        return full.substr(szWorking.size(),full.size()-szWorking.size());
}


//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::ChooseFile
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
void ADN_FileChooser::ChooseFile()
{
    QString qfilename= eMode_==eDirectory ? Q3FileDialog::getExistingDirectory(
                                                        szDirectory_,
                                                        this,
                                                        "get existing directory"
                                                        "Choose a directory")
                                             : Q3FileDialog::getOpenFileName(
                                                                szDirectory_,
                                                                szFilter_,
                                                                this,
                                                                "open file dialog",
                                                                "Choose a file to open" );

    if (qfilename==QString::null)
        return;
    std::string res( qfilename.ascii() );
    std::replace( res.begin(), res.end(), '\\','/' );
    std::string szPartialPath=GetPartPath( szDirectory_.ascii(), res );
    if (szPartialPath.empty())
    {
        QMessageBox::information( this, "ADN",
                                        "Unable to set file.\n"
                                        "File is not in current working directory." );

        return;
    }
    pLineEdit_->setText(szPartialPath.c_str());
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::FilenameChanged
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_FileChooser::FilenameChanged(const QString& file)
{
    static_cast<ADN_FileChooser_Connector*>(vConnectors_[eFile])->SetDataChanged(file);
}


//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::SetFileName
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
void ADN_FileChooser::SetFilename( const QString &fn )
{
    int nPos = pLineEdit_->cursorPosition();
    pLineEdit_->setText( fn );
    pLineEdit_->setCursorPosition( nPos );
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::FileName
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
QString ADN_FileChooser::GetFilename() const
{
    return pLineEdit_->text();
}

//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::SetWorkingDir
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
void ADN_FileChooser::SetDirectory(const QString& szDir)
{
    szDirectory_=szDir;
}


//-----------------------------------------------------------------------------
// Name: ADN_FileChooser::SetMode
// Created: JDY 03-07-01
//-----------------------------------------------------------------------------
void ADN_FileChooser::SetMode( ADN_FileChooser::E_Mode m )
{
    eMode_=m;
}


