// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligenceListView_h_
#define __IntelligenceListView_h_

#include "clients_kernel/IntelligenceHierarchies.h"
#include "clients_gui/HierarchyListView.h"

namespace kernel
{
    class ModelLoaded;
}

namespace gui
{

// =============================================================================
/** @class  IntelligenceListView
    @brief  IntelligenceListView
*/
// Created: SBO 2007-10-18
// =============================================================================
class IntelligenceListView : public HierarchyListView< kernel::IntelligenceHierarchies >
                           , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, EntitySymbols& icons, const kernel::Profile_ABC& profile );
    virtual ~IntelligenceListView();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRename( Q3ListViewItem* item, int col, const QString& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligenceListView( const IntelligenceListView& );            //!< Copy constructor
    IntelligenceListView& operator=( const IntelligenceListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void keyPressEvent( QKeyEvent* event );
    virtual void DisplayIcon( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    EntitySymbols& icons_;
    //@}
};

}

#endif // __IntelligenceListView_h_
