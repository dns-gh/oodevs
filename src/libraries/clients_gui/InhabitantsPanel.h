// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantsPanel_h_
#define __InhabitantsPanel_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_gui/InfoPanel_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class ModelLoaded;
    class InhabitantType;
    class Team_ABC;
}

namespace gui
{
    class PanelStack_ABC;
    class ItemFactory_ABC;
    class InhabitantTypesListView;

// =============================================================================
/** @class  InhabitantsPanel
    @brief  InhabitantsPanel
*/
// Created: SLG 2010-11-23
// =============================================================================
class InhabitantsPanel : public gui::InfoPanel_ABC
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                       , public tools::SelectionObserver_ABC
                       , public tools::SelectionObserver_Base< kernel::Formation_ABC >
                       , public tools::SelectionObserver_Base< kernel::Team_ABC >
{
    Q_OBJECT;
public:

    struct InhabitantPrototype
    {
    public:
        InhabitantPrototype() : type_( 0 ), number_( 0 ) {}
        ~InhabitantPrototype() {}
        const kernel::InhabitantType* type_;
        int number_;
    };
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantsPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::InhabitantType >& types, gui::ItemFactory_ABC& factory );
    virtual ~InhabitantsPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantsPanel( const InhabitantsPanel& );            //!< Copy constructor
    InhabitantsPanel& operator=( const InhabitantsPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void AfterSelection();
    virtual void BeforeSelection();
    virtual void Select( const kernel::Formation_ABC& element );
    virtual void Select( const kernel::Team_ABC& element );
    //@}

private slots:
    //!@name Slots
    //@{
    void OnStartDrag( const kernel::InhabitantType* type );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    InhabitantTypesListView* list_;
    QLineEdit* number_;
    const kernel::Entity_ABC* selected_;
    InhabitantPrototype prototype_;
    //@}
};
}

#endif // __InhabitantsPanel_h_
