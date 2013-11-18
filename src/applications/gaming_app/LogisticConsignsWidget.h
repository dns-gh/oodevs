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

#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "LogisticConsignsWidget_ABC.h"

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

class LogisticsRequestsTable;
class LogisticsRequestsDetailsTable;
class Publisher_ABC;

// =============================================================================
/** @class  LogisticConsignsWidget
    @brief  LogisticConsignsWidget
*/
// Created: MMC 2013-10-21
// =============================================================================
template< typename Extension, typename Request >
class LogisticConsignsWidget : public LogisticConsignsWidget_ABC, 
                               public tools::ElementObserver_ABC< Extension >,
                               public tools::ElementObserver_ABC< Request >,
                               public tools::ElementObserver_ABC< typename Request::History >
{
public:
    //! @name Constructors/Destructor
    //@{
    LogisticConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
        const QString& filter, const kernel::Profile_ABC& profile, Publisher_ABC& publisher, const QStringList& requestsHeader = QStringList() )
        : LogisticConsignsWidget_ABC( parent, controllers, extractor, filter, profile, publisher, requestsHeader ) {}

    ~LogisticConsignsWidget() {}
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifyUpdated( const Extension& consigns )
    {
        if( selected_ && logistic_helpers::HasRetrieveEntityAndSubordinatesUpToBaseLog( *selected_, &consigns ) )
            needUpdating_ = true;
    }

    virtual void NotifyUpdated( const Request& consign )
    {
        SendHistoryRequest( consign );
        DisplayRequest( consign );
    }

    virtual void NotifyUpdated( const typename Request::History& history )
    {
        DisplayHistory( history );
    }

    virtual void DisplayRequests()
    {
        Purge();
        if( !selected_ )
            return;
        std::set< const Request* > consigns;
        logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog( *selected_, [ &consigns ]( const kernel::Entity_ABC& entity ) {
            const Extension* pConsigns = entity.Retrieve< Extension >();
            if( pConsigns )
            {
                consigns.insert( pConsigns->requested_.begin(), pConsigns->requested_.end() );
                consigns.insert( pConsigns->handled_.begin(), pConsigns->handled_.end() );
            }
        } );

        for( auto it = consigns.begin(); it != consigns.end(); ++it )
            DisplayRequest( **it );
    }
    //@}
};

#endif // __LogisticConsignsWidget_h_
