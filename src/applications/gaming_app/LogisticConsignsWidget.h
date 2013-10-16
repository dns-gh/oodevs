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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/SafePointer.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogConsignDisplayer_ABC.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/Simulation.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <boost/bind.hpp>

// =============================================================================
/** @class  LogisticConsignsWidget
    @brief  LogisticConsignsWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
template< typename Consign, typename Extension >
class LogisticConsignsWidget : public LogisticConsignsWidget_ABC
                                 , public tools::ElementObserver_ABC< Extension >
                                 , public tools::ElementObserver_ABC< Consign >
                                 , public LogConsignDisplayer_ABC
                                 , public tools::ElementObserver_ABC< Simulation::sEndTick >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticConsignsWidget( QWidget* parent, kernel::Controllers& controllers, kernel::DisplayExtractor_ABC& extractor );
    virtual ~LogisticConsignsWidget();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayConsign( const Consign& consign, QTreeWidgetItem* pCurrentItem );
    virtual void DisplayTitle( const QString& key, const QString& value );
    virtual void DisplayItem( const QString& key, const QString& value );
    virtual void DisplaySubItemValues( const QString& key, const QString& subKey,
                                       const QMap< QString, T_OrderedValues >& subValues );
    void AddEntityConsignsToSet( kernel::SafePointer< kernel::Entity_ABC > entity, std::set< const Consign* >& requestedConsigns, std::set< const Consign* >& handledConsigns );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticConsignsWidget( const LogisticConsignsWidget& );            //!< Copy constructor
    LogisticConsignsWidget& operator=( const LogisticConsignsWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const Extension& consigns );
    virtual void NotifyUpdated( const Consign& consigns );
    virtual void NotifyUpdated( const Simulation::sEndTick& consigns );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void DisplayConsigns( const std::set< const Consign* >& consigns, QTreeWidgetItem& rootItem );
    QTreeWidgetItem* FindTreeWidgetItem( const Consign& consign, QTreeWidgetItem* rootItem );
    void UpdateConsign( const Consign& consign, QTreeWidgetItem* consignItem );
    void UpdateConsigns();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QTreeWidgetItem* currentItem_;
    bool needUpdating_;
    //@}
};

// -----------------------------------------------------------------------------
template< typename Consign, typename Extension >
struct AddLogisticConsignsToSetFunctor
{
    std::set< const Consign* > requestedConsigns_;
    std::set< const Consign* > handledConsigns_;

    void Add( const kernel::Entity_ABC& entity )
    {
        const Extension* pConsigns = entity.Retrieve< Extension >();
        if( pConsigns )
        {
            requestedConsigns_.insert( pConsigns->requested_.begin(), pConsigns->requested_.end() );
            handledConsigns_.insert( pConsigns->handled_.begin(), pConsigns->handled_.end() );
        }
    }
};
// =============================================================================

#include "LogisticConsignsWidget.inl"

#endif // __LogisticConsignsWidget_h_
