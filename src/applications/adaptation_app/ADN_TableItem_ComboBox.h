//*****************************************************************************
//
// $Created: JDY 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableItem_ComboBox.h $
// $Author: Ape $
// $Modtime: 10/02/05 11:37 $
// $Revision: 2 $
// $Workfile: ADN_TableItem_ComboBox.h $
//
//*****************************************************************************

#ifndef __ADN_TableItem_ComboBox_h_
#define __ADN_TableItem_ComboBox_h_

#include "ADN_Table.h"
#include "ADN_TableItem_ABC.h"
#include "ADN_TableItem_ComboBoxItem.h"
#include "ADN_Connector_Enum.h"
#include "ADN_Connector_Combo_ABC.h"

#pragma warning( push, 0 )
#include <Qt3Support/q3table.h>
#pragma warning( pop )

class ADN_TableItem_ComboBoxItem;

typedef std::vector<ADN_TableItem_ComboBoxItem*>             T_Ptr_TableItem_ComboBoxItem_Vector;
typedef T_Ptr_TableItem_ComboBoxItem_Vector::iterator        IT_Ptr_TableItem_ComboBoxItem_Vector;
typedef T_Ptr_TableItem_ComboBoxItem_Vector::const_iterator  CIT_Ptr_TableItem_ComboBoxItem_Vector;

//*****************************************************************************
// Created: JDY 03-07-21
//*****************************************************************************
class ADN_TableItem_ComboBox
: public ADN_TableItem_ABC
{
public:

    typedef ADN_TableItem_ComboBoxItem T_ItemType;

public:

    explicit ADN_TableItem_ComboBox(ADN_Table* parent,void *data);
    virtual ~ADN_TableItem_ComboBox();

public:

    virtual QWidget *createEditor() const;
    virtual void     setContentFromEditor( QWidget *w );
    virtual void     paint( QPainter *p, const QColorGroup &cg,
                            const QRect &cr, bool selected );
    virtual void setCurrentItem( int i );
    virtual void setCurrentItem( const QString &i );
    int currentItem() const;
    QString currentText() const;
    int count() const;
    QString text( int i ) const;
    virtual void setEditable( bool b );
    bool isEditable() const;
    virtual void setStringList( const QStringList &l );

    int rtti() const;
    static int RTTI;

public:

    //-------------------------------------------------------------------------
    /** @name Methods used by Connector */
    //-------------------------------------------------------------------------
    //@{
    virtual void insertItem ( const QString & t, int index = -1 );
    virtual void insertItem ( ADN_TableItem_ComboBoxItem* item, int index = -1 );
    virtual void changeItem ( const QString & t, int index );
    virtual void removeItem ( int index );
    virtual void clear();
    virtual void SetCurrentData( void*data);
    int          FindNdx( const ADN_TableItem_ComboBoxItem* item ) const;
    int          FindNdx( void * data) const;
    ADN_TableItem_ComboBoxItem*           GetItem( int ndx );
    //@}


protected:

    virtual void        DoValueChanged();

private:
    QComboBox *                             cb;
    QStringList                             entries;
    int                                     current;
    bool                                    edit;
    static QComboBox *                      fakeCombo;
    T_Ptr_TableItem_ComboBoxItem_Vector     vItems_;

};

typedef ADN_Connector_Combo<ADN_TableItem_ComboBox> ADN_Connector_TableItem_ComboBox;
typedef ADN_Connector_Enum<ADN_TableItem_ComboBox>  ADN_Connector_TableItem_EnumComboBox;


#endif // __ADN_TableItem_ComboBox_h_