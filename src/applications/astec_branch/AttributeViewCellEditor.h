// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttributeViewCellEditor_h_
#define __AttributeViewCellEditor_h_

class DataDictionary;
class QTableItem;

// =============================================================================
/** @class  AttributeViewCellEditor
    @brief  AttributeViewCellEditor
*/
// Created: AGE 2006-06-23
// =============================================================================
class AttributeViewCellEditor : public QLineEdit
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AttributeViewCellEditor( QWidget* parent, const DataDictionary& dictionary, QTableItem* myItem );
    virtual ~AttributeViewCellEditor();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTextChanged( const QString& );
    void OnListClicked( QListBoxItem* );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AttributeViewCellEditor( const AttributeViewCellEditor& );            //!< Copy constructor
    AttributeViewCellEditor& operator=( const AttributeViewCellEditor& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const DataDictionary& dictionary_;
    QTableItem* myItem_;
    QPopupMenu* menu_;
    QListBox* list_;
    //@}
};

#endif // __AttributeViewCellEditor_h_
