//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ConfigDialog.h $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 3 $
// $Workfile: ADN_ConfigDialog.h $
//
//*****************************************************************************

#ifndef __ADN_ConfigDialog_h_
#define __ADN_ConfigDialog_h_

#include <QtGui/qdialog.h>

class ADN_Config;
class QLineEdit;
class QPushButton;

//*****************************************************************************
// Created: JDY 03-09-08
//*****************************************************************************
class ADN_ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ADN_ConfigDialog( ADN_Config& config );
    virtual ~ADN_ConfigDialog();

private slots:
    void OnOk();
    void OnCancel();
    void OnBrowsePath();

private:
    ADN_Config* pConfig_;

    QLineEdit*  pSimPath_;
    QPushButton*    pSimPathBrowser_;
    QLineEdit*  pSimArguments_;
    QPushButton*    pOk_;
    QPushButton*    pCancel_;
};

#endif // __ADN_ConfigDialog_h_