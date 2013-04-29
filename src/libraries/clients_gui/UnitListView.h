// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __UnitListView_h_
#define __UnitListView_h_

#include "ListView.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class AgentComposition;
    class AgentTypes;
    class AgentNature;
    class Automat_ABC;
    class AutomatComposition;
    class AutomatType;
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  UnitListView
    @brief  UnitListView
*/
// Created: SBO 2006-08-28
// =============================================================================
class UnitListView : public ListView< UnitListView >
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
             UnitListView( QWidget* parent, kernel::Controllers& controllers, const kernel::AgentTypes& list, ItemFactory_ABC& factory, bool followSelection = false );
    virtual ~UnitListView();
    //@}

    //! @name Operations
    //@{
    void SetOpen( bool open );
    void SetSorting( const std::string& nature );
    void Display( const kernel::AgentComposition& type, ValuedListItem* );
    void Display( const kernel::AutomatComposition& type, ValuedListItem* );
    void Display( const kernel::AutomatType& type, ValuedListItem* );
    virtual Q3DragObject* dragObject();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Agent_ABC& agent );
    virtual void Select( const kernel::Automat_ABC& automat );

    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void DisplayList();
    void DisplayBy( const std::string& (kernel::AgentNature::*function)() const );
    void DisplayByNature();
    Q3ListViewItem* CreateNaturePath( const std::string& path );
    void Sort( Q3ListViewItem* item );

    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                        controllers_;
    const kernel::AgentTypes&                   types_;
    std::string                                 sorting_;
    kernel::SafePointer< kernel::Agent_ABC >    selectedAgent_;
    kernel::SafePointer< kernel::Automat_ABC >  selectedAutomat_;
    const bool                                  followSelection_;
    //@}
};

}

#endif // __UnitListView_h_