//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_DialogLog.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 2 $
// $Workfile: ADN_DialogLog.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_DialogLog.h"
#include "MT_Tools/MT_FormatString.h"

class QMessageTextView : public Q3TextView
{
public:
    QMessageTextView( QWidget *parent, const char *name )
    : Q3TextView( parent, name ) { }
};


//-----------------------------------------------------------------------------
// Name: ADN_DialogLog constructor
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
ADN_DialogLog::ADN_DialogLog( QWidget * parent, const char * name)
: QDialog(parent,name,true,Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title | Qt::WStyle_SysMenu)
, pMsg_(0)
, pText_(0)
, lMsg_()
{
    Q3GridLayout * grid  = new Q3GridLayout( this, 3, 2 ,10 ,10);
    QLabel* pIcon = new QLabel(this);
    pIcon->setPixmap( QMessageBox::standardIcon(QMessageBox::Warning) );
    grid->addWidget(pIcon,0,0);
    pMsg_ = new QLabel(this);
    grid->addWidget(pMsg_,0,1,Qt::AlignLeft);
    pText_= new QMessageTextView( this, "errors" );
    grid->addMultiCellWidget( pText_, 1, 1 , 0 , 1 );
    pOk_ = new QPushButton( tr( "&OK" ), this, "ok" );
    connect( pOk_, SIGNAL(clicked()), this, SLOT(accept()) );
    pOk_->setFocus();
    grid->addMultiCellWidget( pOk_, 2, 2, 0, 1, Qt::AlignCenter );
}


//-----------------------------------------------------------------------------
// Name: ADN_DialogLog destructor
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
ADN_DialogLog::~ADN_DialogLog()
{

}


//-----------------------------------------------------------------------------
// Name: ADN_DialogLog::show
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_DialogLog::show()
{
    pMsg_->setText(szMsg_.c_str() );

    std::string rich;
    for ( std::list<std::string>::iterator it=lMsg_.begin();it!=lMsg_.end();++it )
        rich+=MT_FormatString(szFormat_.c_str(),it->c_str());
    pText_->setText(rich.c_str());

    QDialog::show();
}


//-----------------------------------------------------------------------------
// Name: ADN_DialogLog::done
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_DialogLog::done(int a)
{
    lMsg_.clear();
    QDialog::done(a);
}


