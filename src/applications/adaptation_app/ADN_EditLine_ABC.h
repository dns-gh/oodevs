//*****************************************************************************
//
// $Created: JDY 03-07-08 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_EditLine_ABC.h $
// $Author: Ape $
// $Modtime: 13/04/05 10:23 $
// $Revision: 4 $
// $Workfile: ADN_EditLine_ABC.h $
//
//*****************************************************************************

#ifndef __ADN_EditLine_ABC_h_
#define __ADN_EditLine_ABC_h_

#include "ADN_Gfx_ABC.h"
#include <qlineedit.h>
#include <qvalidator.h>


//*****************************************************************************
// Created: JDY 03-07-08
//*****************************************************************************
class ADN_EditLine_ABC
: public QLineEdit
, public ADN_Gfx_ABC
{
    Q_OBJECT
        
public:
    explicit ADN_EditLine_ABC(QWidget* parent, const char * name = 0);
    virtual ~ADN_EditLine_ABC();

    void setEnabled( bool b );

protected:
    void focusOutEvent( QFocusEvent* pEvent );

protected slots:
    virtual void  TextChanged(const QString& string)=0;
    virtual void UpdateEnableState() = 0;
};


#endif // __ADN_EditLine_ABC_h_