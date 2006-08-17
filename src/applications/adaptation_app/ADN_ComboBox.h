//*****************************************************************************
//
// $Created: JDY 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ComboBox.h $
// $Author: Ape $
// $Modtime: 28/02/05 11:13 $
// $Revision: 4 $
// $Workfile: ADN_ComboBox.h $
//
//*****************************************************************************

#ifndef __ADN_ComboBox_h_
#define __ADN_ComboBox_h_

#include "ADN_Tools.h"
#include "ADN_Types.h"
#include "ADN_Gfx_ABC.h"
#include "ADN_ComboBoxItem.h"
#include "ADN_Connector_ABC.h"
#include "ADN_Connector_Enum.h"
#include "ADN_Connector_Combo_ABC.h"

#include <qcombobox.h>

class ADN_ComboBoxItem;

typedef std::vector<ADN_ComboBoxItem*>            T_PtrComboBoxItem_Vector;
typedef T_PtrComboBoxItem_Vector::iterator        IT_PtrComboBoxItem_Vector;
typedef T_PtrComboBoxItem_Vector::const_iterator  CIT_PtrComboBoxItem_Vector;

//*****************************************************************************
// Created: JDY 03-07-18
//*****************************************************************************
class ADN_ComboBox
:   public QComboBox
,   public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    typedef ADN_ComboBoxItem T_ItemType;

public:
    explicit ADN_ComboBox(QWidget * parent = 0, const char * name = 0);
    virtual ~ADN_ComboBox();

    void                        SetItemConnectors(const T_ConnectorVector& v);

    void*                       GetCurrentData();
    void                        SetCurrentData(void *data);

    virtual void                ConnectItem();
    virtual void                DisconnectItem();
    virtual void                setCurrentItem ( int index );

    void                        insertItem ( ADN_ComboBoxItem* item, int index = -1 );
    void                        insertItem ( const QString& txt, int index = -1 );
    void                        removeItem ( int index );
    void                        clear();
    int                         FindNdx( const ADN_ComboBoxItem* item ) const;
    int                         FindNdx( void * data) const;
    ADN_ComboBoxItem*           GetItem( int ndx );

    void setEnabled( bool b );

signals:
    void Activated( int nIndex );

protected:
    void*                       pCurData_;
    T_ConnectorVector        vItemConnectors_;
    T_PtrComboBoxItem_Vector    vItems_;

private slots:
    void    ItemSelected( int ndx );    

protected slots:
    virtual void UpdateEnableState();
};

typedef ADN_Connector_Combo<ADN_ComboBox> ADN_Connector_ComboBox;
typedef ADN_Connector_Enum<ADN_ComboBox>  ADN_Connector_EnumComboBox;


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::GetCurrentData
// Created: JDY 03-07-18
//-----------------------------------------------------------------------------
inline
void* ADN_ComboBox::GetCurrentData()
{
    return pCurData_;
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox::SetItemConnectors
// Created: JDY 03-09-03
//-----------------------------------------------------------------------------
inline
void ADN_ComboBox::SetItemConnectors(const T_ConnectorVector& v)
{
    vItemConnectors_ = v;
}


#endif // __ADN_ComboBox_h_