//*****************************************************************************
//
// $Created: JDY 03-09-08 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_RunProcessDialog.h $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 3 $
// $Workfile: ADN_RunProcessDialog.h $
//
//*****************************************************************************

#ifndef __ADN_RunProcessDialog_h_
#define __ADN_RunProcessDialog_h_

#include <qdialog.h>

class QTextEdit;
class QProcess;

//*****************************************************************************
// Created: SBO 02-01-2006
//*****************************************************************************
class ADN_RunProcessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ADN_RunProcessDialog( QWidget* pParent, const std::string& strCommandLine, const char* szDialogName );
    virtual ~ADN_RunProcessDialog();

private slots:
    //! @name Slots
    //@{
    void ReadFromStdout();
    void ReadFromStderr();
    void ProcessFinished();
    //@}

private:
    //! @name Copy constructor/assignment operator
    //@{
    ADN_RunProcessDialog( const ADN_RunProcessDialog& );
    ADN_RunProcessDialog& operator=( const ADN_RunProcessDialog& );
    //@}

private:
    QTextEdit* pOutputField_;
    QProcess*  pProcess_;

    const std::string strCommandLine_;

};

#endif // __ADN_RunProcessDialog_h_