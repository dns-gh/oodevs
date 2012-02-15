// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ComboBox_h_
#define __ADN_ComboBox_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_Connector_ABC.h"
#include "ADN_Connector_Enum.h"
#include "ADN_Connector_Combo.h"

class ADN_ComboBoxItem;

typedef std::vector< ADN_ComboBoxItem* > T_PtrComboBoxItem_Vector;

//*****************************************************************************
// Created: JDY 03-07-18
//*****************************************************************************
class ADN_ComboBox : public QComboBox
                   , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    typedef ADN_ComboBoxItem T_ItemType;

public:
    explicit ADN_ComboBox( QWidget * parent = 0 );
    virtual ~ADN_ComboBox();

    void                        SetItemConnectors(const T_ConnectorVector& v);

    void*                       GetCurrentData();
    virtual void                SetCurrentData(void *data);

    virtual void                ConnectItem();
    virtual void                DisconnectItem();
    virtual void                setCurrentItem ( int index );

    virtual void                insertItem( ADN_ComboBoxItem* item, int index = -1 );
    virtual void                insertItem( const QString& txt, int index = -1 );
    void                        removeItem( int index );
    void                        clear();
    int                         FindNdx( const ADN_ComboBoxItem* item ) const;
    int                         FindNdx( void * data) const;
    ADN_ComboBoxItem*           GetItem( int ndx );
    int                         GetEnumIndexFromGUI( int index ) const;

    virtual void wheelEvent( QWheelEvent* );

protected:
    void*                       pCurData_;
    T_ConnectorVector           vItemConnectors_;
    T_PtrComboBoxItem_Vector    vItems_;
    std::vector< int >          vItemsEnum_;
    bool                        fromView_;

private slots:
    void    ItemSelected( int ndx );
};

typedef ADN_Connector_Combo< ADN_ComboBox > ADN_Connector_ComboBox;
typedef ADN_Connector_Enum< ADN_ComboBox >  ADN_Connector_EnumComboBox;

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

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox::GetEnumIndexFromGUI
// Created: ABR 2011-01-13
// -----------------------------------------------------------------------------
inline
int ADN_ComboBox::GetEnumIndexFromGUI( int index ) const
{
    return ( vItemsEnum_.empty() ) ? index : vItemsEnum_.at( index );
}


#endif // __ADN_ComboBox_h_