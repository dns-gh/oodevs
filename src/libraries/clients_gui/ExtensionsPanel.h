// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_ExtensionsPanel_h_
#define gui_ExtensionsPanel_h_

#include "RichDockWidget.h"
#include "clients_kernel/SafePointer.h"
#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Agent_ABC;
    class AttributeType;
    class Controllers;
    class DictionaryExtensions;
    class Entity_ABC;
    class ExtensionTypes;
    class Formation_ABC;
    class Profile_ABC;
}

namespace gui
{
    class DiffusionListDialog;
    class EntitySymbols;
    class RichGroupBox;

// =============================================================================
/** @class  ExtensionsPanel
    @brief  ExtensionsPanel
*/
// Created: ABR 2011-05-10
// =============================================================================
class ExtensionsPanel : public RichDockWidget
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::DictionaryExtensions >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionsPanel( QMainWindow* parent, kernel::Controllers& controllers, 
                              const kernel::ExtensionTypes& extensions, const tools::Resolver< kernel::Agent_ABC >& agents,
                              const tools::Resolver< kernel::Formation_ABC >& formations, const kernel::Profile_ABC& profile );
    virtual ~ExtensionsPanel();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    virtual void NotifyUpdated( const kernel::DictionaryExtensions& extensions );
    void AddWidget( const kernel::AttributeType& attribute, int currentRow );
    void DeleteWidgets();
    void UpdateDisplay();
    void UpdateDependencies();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnActivationChanged( bool );
    virtual void Commit();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, QWidget* > T_Widgets;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    const kernel::ExtensionTypes& extensions_;
    const kernel::Profile_ABC& profile_;
    std::auto_ptr< DiffusionListDialog > diffusionDialog_;
    QVBoxLayout* pMainLayout_;
    QWidget* pExtensionLayout_;
    RichGroupBox* pGroupBox_;
    QGridLayout* pGroupBoxLayout_;
    T_Widgets widgets_;
    bool updating_;
    //@}
};

}
#endif // gui_ExtensionsPanel_h_
