// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsignsWidget_ABC_h_
#define __LogisticConsignsWidget_ABC_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "gaming/LogisticsConsign_ABC.h"
#include "LogisticsRequestsHistoryTable.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class DisplayExtractor;
    class RichCheckBox;
}

class LogisticSelectionDialog_ABC;
class LogisticsRequestsTable;
class LogisticsRequestsDetailsTable;
class SimulationController;
class Model;
class LogisticsModel;

// =============================================================================
/** @class  LogisticConsignsWidget_ABC
    @brief  LogisticConsignsWidget_ABC
*/
// Created: MMC 2012-10-29
// =============================================================================
class LogisticConsignsWidget_ABC : public QWidget
                                 , public tools::Observer_ABC
                                 , public tools::SelectionObserver< kernel::Entity_ABC >
                                 , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticConsignsWidget_ABC( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                         const kernel::Profile_ABC& profile, const SimulationController& simulationController,
                                         Model& model, const QStringList& requestsHeader = QStringList() );
    virtual ~LogisticConsignsWidget_ABC();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Purge();
    virtual void FillCurrentModel( const kernel::Entity_ABC& entity ) = 0;
    virtual void FillHistoryModel() = 0;
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    void OnRequestsTableSelected( const QModelIndex& current, const QModelIndex& previous );
    void OnCompletedFilter();
    void OnSelectionDialogRequested( const LogisticsConsign_ABC& consign ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void PurgeDetail();
    virtual void OnRequestSelected( const LogisticsConsign_ABC& consign ) = 0;
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void DisplayHistory( const LogisticsConsign_ABC::History& history );
    virtual void DisplayRequest( const LogisticsConsign_ABC& consign );
    virtual bool IsHistoryChecked() const;
    virtual void DisplayCurrentHistory();
    virtual void SelectRequest();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void DisplayRequestHistory( const LogisticsConsign_ABC& consign );
    void SendHistoryRequest( const LogisticsConsign_ABC& consign );
    QString GetDisplayName( const kernel::Entity_ABC* pEntity ) const;
    void UpdateEntityDetails( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* detailEntity, const QString& key );
    QString SupervisionFilter( const QString& value ) const;
    void SendHistoryRequests();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::DisplayExtractor& extractor_;
    const LogisticsModel& historyModel_;
    const kernel::Profile_ABC& profile_;
    const SimulationController& simulationController_;
    QVBoxLayout* pDetailLayout_;
    LogisticsRequestsTable* requestsTable_;
    LogisticsRequestsDetailsTable* detailsTable_;
    LogisticsRequestsHistoryTable* historyTable_;
    gui::RichCheckBox* completedCheckbox_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    bool needUpdating_;
    const LogisticsConsign_ABC* requestSelected_;
    unsigned int lastRequestId_;
    std::unique_ptr< LogisticSelectionDialog_ABC > selectionDialog_;
    //@}
};

#endif // __LogisticConsignsWidget_ABC_h_
