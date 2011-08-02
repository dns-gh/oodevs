//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_CheckBox.h $
// $Author: Ape $
// $Modtime: 30/12/04 15:07 $
// $Revision: 2 $
// $Workfile: ADN_CheckBox.h $
//
//*****************************************************************************

#ifndef __ADN_CheckBox_h_
#define __ADN_CheckBox_h_

#include "ADN_Gfx_ABC.h"
#include <QtGui/qcheckbox.h>


//*****************************************************************************
// Created: JDY 03-07-02
//*****************************************************************************
class ADN_CheckBox
: public QCheckBox
, public ADN_Gfx_ABC
{
    Q_OBJECT

public:

    explicit ADN_CheckBox(QWidget* parent, const char * name = 0);
    explicit ADN_CheckBox(const QString &text, QWidget *parent, const char* name=0);
    virtual ~ADN_CheckBox();

    void setEnabled( bool b );

private slots:

    void    BoolChanged(bool b);
    void UpdateEnableState();
};

#endif // __ADN_CheckBox_h_