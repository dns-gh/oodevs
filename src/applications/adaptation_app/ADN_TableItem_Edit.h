//*****************************************************************************
//
// $Created: JDY 03-07-09 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableItem_Edit.h $
// $Author: Ape $
// $Modtime: 14/04/05 15:09 $
// $Revision: 5 $
// $Workfile: ADN_TableItem_Edit.h $
//
//*****************************************************************************

#ifndef __ADN_TableItem_Edit_h_
#define __ADN_TableItem_Edit_h_

#include "ADN_Validator.h"
#include "ADN_Table.h"
#include "ADN_TableItem_ABC.h"
#include "ADN_Connector_String.h"
#include "ADN_Connector_Int.h"
#include "ADN_Connector_Double.h"

#pragma warning( push, 0 )
#include <Qt3Support/q3table.h>
#pragma warning( pop )

//*****************************************************************************
// Created: JDY 03-07-09
//*****************************************************************************
template <class Connector,class Validator>
class ADN_TableItem_Edit
:public ADN_TableItem_ABC
{
public:
    typedef Validator   T_Validator;
    typedef Connector   T_Connector ;

public:
    explicit ADN_TableItem_Edit(ADN_Table* parent,void *data, Q3TableItem::EditType nEditType = OnTyping );
    virtual ~ADN_TableItem_Edit();

    Validator& GetValidator();
    void SetValidator( Validator* pValidator );

    void UseColor( bool bUseColor );
    void SetRangeForColor( double rMin, double rMax );
    virtual void paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected );

    void SetAlignment( int nAlignment );
    int alignment() const;

protected:
    virtual QWidget* createEditor() const;
    virtual void DoValueChanged();

private:
    Validator* pValidator_;

    bool bUseColor_;
    double rMin_;
    double rMax_;

    int nAlignment_;
};

#include "ADN_TableItem_Edit.inl"

typedef ADN_TableItem_Edit< ADN_Connector_String< ADN_TableItem_ABC > , QRegExpValidator >        ADN_TableItem_String;
typedef ADN_TableItem_Edit< ADN_Connector_Int< ADN_TableItem_ABC >    , ADN_IntValidator    >        ADN_TableItem_Int;
typedef ADN_TableItem_Edit< ADN_Connector_Double< ADN_TableItem_ABC > , ADN_DoubleValidator >     ADN_TableItem_Double;
typedef ADN_TableItem_Edit< ADN_Connector_Double< ADN_TableItem_ABC > , ADN_PercentageValidator > ADN_TableItem_Percentage;

#endif // __ADN_TableItem_Edit_h_