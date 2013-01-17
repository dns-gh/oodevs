// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_PropertiesPanel_h_
#define __gui_PropertiesPanel_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class EditorFactory_ABC;
    class DictionaryUpdated;
    class UrbanObject_ABC;
    class PropertiesGroupDictionary;
}

namespace gui
{
    class GlProxy;
    class PropertyDisplayer;
    class PropertyTreeView;
    class PropertyModel;
    class PropertyDelegate;

// =============================================================================
/** @class  PropertiesPanel
    @brief  Panel to display Entity properties registered into
            PropertiesDictionary extension
*/
// Created: SBO 2008-04-08
// =============================================================================
class PropertiesPanel : public QScrollArea
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::DictionaryUpdated >
                      , public kernel::MultipleSelectionObserver< kernel::UrbanObject_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, kernel::EditorFactory_ABC& factory,
                              PropertyDisplayer& displayer, PropertyDisplayer& comparator, const GlProxy& glProxy );
    virtual ~PropertiesPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifySelectionChanged( const std::vector< const kernel::UrbanObject_ABC* >& elements );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    virtual void NotifyUpdated( const kernel::DictionaryUpdated& message );
    virtual void NotifyDeleted( const kernel::DictionaryUpdated& message );
    virtual void NotifyCreated( const kernel::DictionaryUpdated& message );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemChanged();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                               controllers_;
    const GlProxy&                                     glProxy_;
    PropertyTreeView*                                  view_;
    PropertyModel*                                     model_;
    PropertyDelegate*                                  delegate_;
    const kernel::Entity_ABC*                          selected_;
    std::auto_ptr< kernel::PropertiesGroupDictionary > pMultiProperties_;
    //@}
};

}

#endif // __gui_PropertiesPanel_h_
