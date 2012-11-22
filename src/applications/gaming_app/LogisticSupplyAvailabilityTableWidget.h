// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
    void SetQuantities( QMap< QString, int >& quantities, const QMap< QString, int >& maxQuantities );

private slots:
    //! @name Slots
    //@{
    void OnNameChanged( const QString& newName );
    //@}

private:

    //! @name Member data
    //@{
    QMap< QString, int > maxQuantities_;
    QStringList header_;
    //@}

};

#endif // __LogisticSupplyAvailabilityTableWidget_h_
