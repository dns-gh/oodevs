// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsignsWidget_h_
#define __LogisticConsignsWidget_h_

#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "LogisticConsignsWidget_ABC.h"
#include "LogisticsRequestsTable.h"
#include "gaming/LogisticsModel.h"

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
class SimulationController;
class LogisticsModel;

// =============================================================================
/** @class  LogisticConsignsWidget
    @brief  LogisticConsignsWidget
*/
// Created: MMC 2013-10-21
// =============================================================================
template< typename Extension, typename Request >
class LogisticConsignsWidget : public LogisticConsignsWidget_ABC,
                               public tools::ElementObserver_ABC< typename Request::History >
{
public:
    //! @name Constructors/Destructor
    //@{
    LogisticConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                            const kernel::Profile_ABC& profile, const SimulationController& simulationController, Model& model,
                            E_LogisticChain type, const QStringList& requestsHeader = QStringList() )
        : LogisticConsignsWidget_ABC( parent, controllers, extractor, profile, simulationController, model, requestsHeader )
        , type_( type )
        {}
    ~LogisticConsignsWidget()
        {}
    //@}

public:
    //! @name Operations
    //@{
    virtual void FillCurrentModel( const kernel::Entity_ABC& entity )
    {
        if( !IsHistoryChecked() )
        {
            Purge();
            std::set< const Request* > consigns;
            logistic_helpers::VisitEntityAndSubordinatesUpToBaseLog( entity, [ &consigns ]( const kernel::Entity_ABC& entity ) {
                const Extension* pConsigns = entity.Retrieve< Extension >();
                if( pConsigns )
                {
                    consigns.insert( pConsigns->requested_.begin(), pConsigns->requested_.end() );
                    consigns.insert( pConsigns->handled_.begin(), pConsigns->handled_.end() );
                }
            } );

            for( auto it = consigns.begin(); it != consigns.end(); ++it )
                DisplayRequest( **it );
            requestsTable_->ResizeColumnsToContents();
            SendHistoryRequests();
            SelectRequest();
        }
    }

    virtual void FillHistoryModel()
    {
        if( IsHistoryChecked() )
        {
            Purge();
            auto it = historyModel_.tools::Resolver< Request >::CreateIterator();
            while( it.HasMoreElements() )
                DisplayRequest( it.NextElement() );
            requestsTable_->ResizeColumnsToContents();
            SendHistoryRequests();
            SelectRequest();
        }
    }
    //@}

protected:
    //! @name Operations
    //@{
    virtual void NotifyUpdated( const typename Request::History& history )
    {
        if( history.GetConsign().GetType() == type_ )
            DisplayHistory( history );
    }
    //@}

private:
    //! @name Member data
    //@{
    E_LogisticChain type_;
    //@}
};

#endif // __LogisticConsignsWidget_h_
