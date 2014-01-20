// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyAvailabilityTableWidget_h_
#define __LogisticSupplyAvailabilityTableWidget_h_

// =============================================================================
/** @class  LogisticSupplyAvailabilityItemDelegate
    @brief  LogisticSupplyAvailabilityItemDelegate
*/
// Created: MMC 2012-10-11
// =============================================================================
class LogisticSupplyAvailabilityItemDelegate : public QItemDelegate
{
    Q_OBJECT;

public:
    LogisticSupplyAvailabilityItemDelegate( QWidget* parent = 0 );
    ~LogisticSupplyAvailabilityItemDelegate();

    virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index ) const;
    virtual void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;

private:

    bool IsNameInTable( const QAbstractItemModel* model, const QString& name ) const;
};

// =============================================================================
/** @class  LogisticSupplyAvailabilityTableWidget
    @brief  LogisticSupplyAvailabilityTableWidget
*/
// Created: MMC 2012-10-11
// =============================================================================
class LogisticSupplyAvailabilityTableWidget: public QTableWidget
{
    Q_OBJECT;

public:
    LogisticSupplyAvailabilityTableWidget( QWidget* parent, const QStringList& header );
    ~LogisticSupplyAvailabilityTableWidget();

    void Clear();
    const QMap< QString, int >& GetMaxQuantities() const;
    void GetQuantities( QMap< QString, int >& quantities ) const;
    void SetQuantities( const QMap< QString, int >& quantities, const QMap< QString, int >& maxQuantities );

signals:
    void OnChanged( int row );

protected:
    virtual void AddRow( int row, const QString& newName, int available );

    void SetContent( int row, int col, double value );
    void SetContent( int row, int col, const QString& value );

private slots:
    //! @name Slots
    //@{
    void OnNameChanged( const QString& newName );
    void OnQuantityChanged();
    //@}

private:
    //! @name Member data
    //@{
    QMap< QString, int > maxQuantities_;
    QStringList header_;
    //@}

};

#endif // __LogisticSupplyAvailabilityTableWidget_h_
