// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsignsWidget_h_
#define __LogisticConsignsWidget_h_

#include "LogisticConsignsWidget_ABC.h"
#include "LogisticsRequestsTable.h"
#include "clients_kernel/Entity_ABC.h"
#include "gaming/LogisticsModel.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

class SimulationController;
class LogisticsModel;

// =============================================================================
/** @class  LogisticConsignsWidget
    @brief  LogisticConsignsWidget
*/
// Created: MMC 2013-10-21
// =============================================================================
template< typename Extension, typename Request >
class LogisticConsignsWidget : public LogisticConsignsWidget_ABC
                             , public tools::ElementObserver_ABC< typename Request::History >
                             , public tools::ElementObserver_ABC< Request >
{
public:
    LogisticConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                            const kernel::Profile_ABC& profile, const SimulationController& simulationController, Model& model,
                            E_LogisticChain type, const QStringList& requestsHeader = QStringList() )
        : LogisticConsignsWidget_ABC( parent, controllers, extractor, profile, simulationController, model, requestsHeader )
        , type_( type )
    {
        controllers_.Update( *this );
    }
    virtual ~LogisticConsignsWidget()
    {
        controllers_.Unregister( *this );
    }

public:
    virtual void FillHistoryModel()
    {
        Purge();
        const bool history = IsHistoryChecked();
        auto it = historyModel_.tools::Resolver< Request >::CreateIterator();
        while( it.HasMoreElements() )
        {
            const auto& request = it.NextElement();
            if( history || IsActive( request ) )
                DisplayRequest( request );
        }
        requestsTable_->resizeColumnsToContents();
        SendHistoryRequests();
        SelectRequest();
    }

protected:
    virtual void NotifyUpdated( const typename Request::History& history )
    {
        if( history.GetConsign().GetType() == type_ )
            DisplayHistory( history );
    }
    virtual void NotifyDeleted( const Request& request )
    {
        RemoveRequest( request );
        if( requestSelected_ == &request )
            requestSelected_ = 0;
    }

private:
    virtual bool IsActive( const Request& ) const = 0;

private:
    E_LogisticChain type_;
};

#endif // __LogisticConsignsWidget_h_
