// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticsRequestsTable_h_
#define __LogisticsRequestsTable_h_

#include "clients_kernel/Tools.h"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/RichTableView.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class LinkItemDelegate;
}

class LogisticsConsign_ABC;
class LogisticsModel;

// =============================================================================
/** @class  LogisticsRequestsTable
    @brief  LogisticsRequestsTable
*/
// Created: MMC 2013-09-11
// =============================================================================
class LogisticsRequestsTable : public gui::RichTableView
                             , public tools::Observer_ABC
                             , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Types
    //@{
    typedef std::function< bool( const LogisticsConsign_ABC&, const kernel::Entity_ABC& ) > T_IsEntityInConsignFunctor;
    typedef std::function< QString( const LogisticsConsign_ABC& ) >                         T_RequesterNameExtractor;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             LogisticsRequestsTable( const QString& objectName,
                                     const QStringList& horizontalHeaders,
                                     const kernel::Controllers& controllers,
                                     const kernel::Profile_ABC& profile,
                                     const LogisticsModel& historyModel,
                                     QWidget* parent = 0 );
    virtual ~LogisticsRequestsTable();
    //@}

public:
    //! @name Operations
    //@{
    void AddRequest( const LogisticsConsign_ABC& consign );
    void RemoveRequest( const LogisticsConsign_ABC& consign );
    void SelectRequest( unsigned int id );
    const LogisticsConsign_ABC* GetRequest( const QModelIndex& index ) const;
    const LogisticsConsign_ABC* GetCurrentRequest() const;
    void FindRequestsIds( std::set< unsigned int >& requests );
    void SetData( int col, QString text, const LogisticsConsign_ABC& consign );
    void SetIsEntityInConsignFunctor( const T_IsEntityInConsignFunctor& functor );
    void SetRequesterNameExtractor( const T_RequesterNameExtractor& extractor );
    //@}

private:
    //! @name Helpers
    //@{
     int GetRequestRow( unsigned int consignId );
     void SetData( int row, int col, QString displayText,
                   QVariant sortText, unsigned int consignId);
     virtual void NotifyUpdated( const kernel::Entity_ABC& element );
    //@}

signals:
    //! @name Signals
    //@{
     void RequestSelectionDialog( const LogisticsConsign_ABC& consign );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    //@}

protected:
    //! @name Data Members
    //@{
    gui::LinkItemDelegate*     linkItemDelegate_;
    const kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    const LogisticsModel& historyModel_;
    T_IsEntityInConsignFunctor isEntityInConsignFunctor_;
    T_RequesterNameExtractor requesterNameExtractor_;
    //@}
};

#endif // __LogisticsRequestsTable_h_
