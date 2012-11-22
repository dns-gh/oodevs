// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSupplyExclusiveListWidget_h_
#define __LogisticSupplyExclusiveListWidget_h_

// =============================================================================
/** @class  LogisticSupplyExclusiveListWidget
    @brief  LogisticSupplyExclusiveListWidget
*/
// Created: MMC 2012-10-11
// =============================================================================
class LogisticSupplyExclusiveListWidget : public QListWidget
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    LogisticSupplyExclusiveListWidget( QWidget* parent, const QString& addLabel, const QString& removeLabel );
    virtual ~LogisticSupplyExclusiveListWidget();
    //@}

    //! @name Operations
    //@{
    void SetChoice( const QStringList& choice );
    void GetItems( QStringList& items );
    void Clear();
    //@}

signals:
    //! @name signals
    //@{
    void ItemAdded( const QString& item );
    void ItemRemoved( const QString& item );
    void SelectionChanged( const QString& current, const QString& previous );
    //@}

private:
    //! @name Operations
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* event );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTriggeredAction( QAction* action );
    void OnSelectionChanged( QListWidgetItem *current, QListWidgetItem *previous );
    //@}

private:
    //! @name Member data
    //@{
    QAction* removeAction_;
    QStringList choice_;
    QStringList choosen_;
    QString addLabel_;
    QString removeLabel_;
    //@}
};

#endif // __LogisticSupplyExclusiveListWidget_h_
