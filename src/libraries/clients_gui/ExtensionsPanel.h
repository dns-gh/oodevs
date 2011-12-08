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

#include "clients_kernel/SafePointer.h"
#include "tools/SelectionObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Agent_ABC;
    class AttributeType;
    class Controllers;
    class DictionaryExtensions;
    class Entity_ABC;
    class ExtensionTypes;
    class Profile_ABC;
}

namespace gui
{
    class DiffusionListDialog;
    class EntitySymbols;
    class ItemFactory_ABC;

// =============================================================================
/** @class  ExtensionsPanel
    @brief  ExtensionsPanel
*/
// Created: ABR 2011-05-10
// =============================================================================
class ExtensionsPanel : public QDockWidget
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::DictionaryExtensions >
                      , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionsPanel( QMainWindow* parent, kernel::Controllers& controllers, const kernel::ExtensionTypes& extensions, const tools::Resolver< kernel::Agent_ABC >& agents,
                              ItemFactory_ABC& factory, const EntitySymbols& icons, const kernel::Profile_ABC& profile, const char* name = 0 );
    virtual ~ExtensionsPanel();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    virtual void NotifyUpdated( const kernel::DictionaryExtensions& extensions );
    void AddWidget( const kernel::AttributeType& attribute );
    void DeleteWidgets();
    void UpdateDisplay();
    void UpdateDependencies();
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnActivationChanged( bool );
    virtual void Commit();
    virtual void OnChangeNationality() {}
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< QWidget* >       T_Widgets;
    typedef T_Widgets::const_iterator   CIT_Widgets;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Entity_ABC* selected_;
    const kernel::ExtensionTypes& extensions_;
    DiffusionListDialog* diffusionDialog_;
    Q3VBox* pMainLayout_;
    Q3VBox* pExtensionLayout_;
    Q3GroupBox* pGroupBox_;
    T_Widgets widgets_;
    bool updating_;
    //@}
};

}
#endif // gui_ExtensionsPanel_h_
