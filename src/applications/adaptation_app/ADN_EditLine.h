//*****************************************************************************
//
// $Created: JDY 03-07-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_EditLine.h $
// $Author: Ape $
// $Modtime: 14/04/05 16:14 $
// $Revision: 5 $
// $Workfile: ADN_EditLine.h $
//
//*****************************************************************************

#ifndef __ADN_EditLine_h_
#define __ADN_EditLine_h_

#include "ADN_Validator.h"
#include "ADN_EditLine_ABC.h"
#include "ADN_Connector_String.h"
#include "ADN_Connector_Int.h"
#include "ADN_Connector_Double.h"


//*****************************************************************************
// Created: JDY 03-07-01
//*****************************************************************************
template <class Connector, class Validator>
class ADN_EditLine
: public ADN_EditLine_ABC
{
public:
    explicit ADN_EditLine(QWidget* parent, const char * name = 0);
    virtual ~ADN_EditLine();

    Validator& GetValidator();

protected:
    virtual void TextChanged(const QString& string);
    virtual void UpdateEnableState();

private:
    Validator* pValidator_;
};

#include "ADN_EditLine.inl"

typedef ADN_EditLine< ADN_Connector_String<ADN_EditLine_ABC>, QRegExpValidator > ADN_EditLine_String;
typedef ADN_EditLine< ADN_Connector_Int<ADN_EditLine_ABC>, ADN_IntValidator >       ADN_EditLine_Int;
typedef ADN_EditLine< ADN_Connector_Double<ADN_EditLine_ABC>, ADN_DoubleValidator > ADN_EditLine_Double;


#endif // __ADN_EditLine_h_