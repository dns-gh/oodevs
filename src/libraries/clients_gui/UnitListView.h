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
    class AgentComposition;
    class AgentTypes;
    class AutomatComposition;
    class AutomatType;
    class ModelLoaded;
    class ModelUnLoaded;
    class Controllers;
    class AgentNature;
    class ComponentType;
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
{

public:
    //! @name Constructors/Destructor
    //@{
             UnitListView( QWidget* parent, kernel::Controllers& controllers, const kernel::AgentTypes& list, ItemFactory_ABC& factory );
    virtual ~UnitListView();
    //@}

    //! @name Operations
    //@{
    void SetOpen( bool open );
    void SetSorting( const std::string& nature );
    void Display( const kernel::ComponentType& type, ValuedListItem* );
    void Display( const kernel::AgentComposition& type, ValuedListItem* );
    void Display( const kernel::AutomatComposition& type, ValuedListItem* );
    void Display( const kernel::AutomatType& type, ValuedListItem* );
    virtual QDragObject* dragObject();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    UnitListView( const UnitListView& );            //!< Copy constructor
    UnitListView& operator=( const UnitListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void DisplayList();
    void DisplayBy( const std::string& (kernel::AgentNature::*function)() const );
    void DisplayByNature();
    QListViewItem* CreateNaturePath( const std::string& path );
    void Sort( QListViewItem* item );

    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::AgentTypes& types_;
    std::string sorting_;
    //@}
};

}

#endif // __UnitListView_h_
