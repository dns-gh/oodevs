// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_UnitTreeView_h_
#define __gui_UnitTreeView_h_

#include "RichTreeView.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class AgentNature;
    class AgentType;
    class AgentTypes;
    class AutomatType;
    class ModelLoaded;
    class ModelUnLoaded;
    class Agent_ABC;
    class Automat_ABC;
}

namespace gui
{

// =============================================================================
/** @class  UnitTreeView
    @brief  UnitTreeView
*/
// Created: JSR 2012-09-21
// =============================================================================
class UnitTreeView : public gui::RichTreeView
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                   , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                   , public tools::SelectionObserver_ABC
                   , public tools::SelectionObserver_Base< kernel::Agent_ABC >
                   , public tools::SelectionObserver_Base< kernel::Automat_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             UnitTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::AgentTypes& types, QWidget* parent, bool selectOnlyAutomats = false );
    virtual ~UnitTreeView();
    //@}

    //! @name Operations
    //@{
    void SetSorting( const std::string& nature );
    template< typename T >
    T* GetSelected() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual QStringList MimeTypes() const;
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void FillList();
    void FillListBy( const std::string&( kernel::AgentNature::*function )() const );
    QStandardItem* CreateNaturePath( const std::string& path );
    void FillListByNature();
    void FillAutomatComposition( QStandardItem& parent, const kernel::AutomatType& type );
    void FillAgentComposition( QStandardItem& parent, const kernel::AgentType& type );

    virtual void BeforeSelection() {}
    virtual void AfterSelection() {}
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Agent_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::AgentTypes& types_;
    bool selectOnlyAutomats_;
    std::string sorting_; // TODO replace by an enumeration?
    //@}
};

// -----------------------------------------------------------------------------
// Name: UnitTreeView::GetSelected
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
template< typename T >
T* UnitTreeView::GetSelected() const
{
    QStandardItem* item = dataModel_.GetItemFromIndex( selectionModel()->currentIndex() );
    if( item && item->data( Roles::MimeTypeRole ).isValid() && item->data( Roles::MimeTypeRole ).toString() == typeid( T ).name() )
        return dataModel_.GetDataFromItem< T >( *item );
    return 0;
}

}

#endif // __gui_UnitTreeView_h_
