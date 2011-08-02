//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_DialogLog.h $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 3 $
// $Workfile: ADN_DialogLog.h $
//
//*****************************************************************************

#ifndef __ADN_DialogLog_h_
#define __ADN_DialogLog_h_


#include <QtGui/qdialog.h>
//Added by qt3to4:
#include <Qt3Support/q3button.h>

class QLabel;
class QPushButton;
class QMessageTextView;

//*****************************************************************************
// Created: JDY 03-09-08
//$$$$ Cette classe est hautement spécifique et n'est utilisée qu'à un endroit
//$$$$ Vu son interet limité, la virer.
//*****************************************************************************
class ADN_DialogLog
: public QDialog
{
public:
    explicit ADN_DialogLog( QWidget * parent = 0, const char * name = 0);
    virtual ~ADN_DialogLog();

    void setMsg(const std::string& msg);
    void setMsgFormat(const std::string& msg);
    void addMsg(const std::string& msg);

    bool         empty() const;

    virtual void show();
    virtual void done(int);

private:

    std::string             szMsg_;
    std::string             szFormat_;
    std::list<std::string>  lMsg_;
    QLabel*                 pMsg_;
    QMessageTextView*       pText_;
    QPushButton*                pOk_;
};


//-----------------------------------------------------------------------------
// Name: ADN_DialogLog::SetMsg
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
inline
void ADN_DialogLog::setMsg(const std::string& msg)
{
    szMsg_=msg;
}


//-----------------------------------------------------------------------------
// Name: ADN_DialogLog::addMsg
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
inline
void ADN_DialogLog::addMsg(const std::string& msg)
{
    lMsg_.push_back(msg);
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
inline
void ADN_DialogLog::setMsgFormat(const std::string& msg)
{
    szFormat_=msg;
}


//-----------------------------------------------------------------------------
// Name: ADN_DialogLog::empty
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
inline
bool ADN_DialogLog::empty() const
{
    return lMsg_.empty();
}

#endif // __ADN_DialogLog_h_