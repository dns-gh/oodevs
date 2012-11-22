// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyValuesTableWidget_h_
#define __LogisticSupplyValuesTableWidget_h_

// =============================================================================
/** @class  LogisticSupplyValueItemDelegate
    @brief  LogisticSupplyValueItemDelegate
*/
// Created: MMC 2012-10-11
// =============================================================================
class LogisticSupplyValueItemDelegate : public QItemDelegate
{
    Q_OBJECT;

public:
    LogisticSupplyValueItemDelegate( QWidget* parent = 0 );
    ~LogisticSupplyValueItemDelegate();

    virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index ) const;
    virtual void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;

private:

    bool IsNameInTable( const QAbstractItemModel* model, const QString& name ) const;
};

// =============================================================================
/** @class  LogisticSupplyValuesTableWidget
    @brief  LogisticSupplyValuesTableWidget
*/
// Created: MMC 2012-10-11
// =============================================================================
class LogisticSupplyValuesTableWidget: public QTableWidget
{
    Q_OBJECT;

public:

    LogisticSupplyValuesTableWidget( QWidget* parent, const QStringList& header );
    ~LogisticSupplyValuesTableWidget();

    void Clear();
    const QStringList& GetPossible() const;
    void GetQuantities( QMap< QString, int >& quantities ) const;
    void SetQuantities( QMap< QString, int >& quantities, QStringList& possible );

private slots:
    //! @name Slots
    //@{
    void OnNameChanged( const QString& newName );
    //@}

private:

    //! @name Member data
    //@{
    QStringList possible_;
    QStringList header_;
    //@}

};

#endif // __LogisticSupplyValuesTableWidget_h_
