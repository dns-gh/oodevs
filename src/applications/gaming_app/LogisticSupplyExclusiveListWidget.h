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

#include <tools/Observer_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Controllers;
}

// =============================================================================
/** @class  LogisticSupplyExclusiveListWidget
    @brief  LogisticSupplyExclusiveListWidget
*/
// Created: MMC 2012-10-11
// =============================================================================
class LogisticSupplyExclusiveListWidget : public QListWidget
                                        , public tools::Observer_ABC
                                        , public tools::ElementObserver_ABC< kernel::Automat_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSupplyExclusiveListWidget( kernel::Controllers& controllers,
                                                QWidget* parent,
                                                const QString& addLabel,
                                                const QString& removeLabel );
    virtual ~LogisticSupplyExclusiveListWidget();
    //@}

    //! @name Operations
    //@{
    void AddChoice( const kernel::Automat_ABC& automat );
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
    virtual void NotifyUpdated( const kernel::Automat_ABC& automat );
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
    kernel::Controllers& controllers_;
    QAction* removeAction_;
    std::vector< const kernel::Automat_ABC* > choice_;
    std::vector< const kernel::Automat_ABC* > choosen_;
    QString addLabel_;
    QString removeLabel_;
    //@}
};

#endif // __LogisticSupplyExclusiveListWidget_h_
