// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __OrbatAttributesPanel_h_
#define __OrbatAttributesPanel_h_

#include <boost/noncopyable.hpp>
#include "tools/SelectionObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class AttributeType;
    class Controllers;
    class DictionaryExtensions;
    class Entity_ABC;
    class ExtensionTypes;
}

class DiffusionListDialog;

// =============================================================================
/** @class  OrbatAttributesPanel
    @brief  OrbatAttributesPanel
*/
// Created: JSR 2010-10-04
// =============================================================================
class OrbatAttributesPanel : public QDockWindow
                           , public tools::Observer_ABC
                           , public tools::SelectionObserver< kernel::Entity_ABC >
                           , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                           , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             OrbatAttributesPanel( QMainWindow* parent, kernel::Controllers& controllers, const kernel::ExtensionTypes& extensions, DiffusionListDialog& diffusionDialog );
    virtual ~OrbatAttributesPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< QWidget* >       T_Widgets;
    typedef T_Widgets::const_iterator   CIT_Widgets;
    //@}

private:
    //! @name Helpers
    //@{
    void AddWidget( const kernel::AttributeType& attribute );
    void DeleteWidgets();
    void UpdateDependencies();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnActivationChanged( bool );
    void Commit();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                      controllers_;
    const kernel::ExtensionTypes&             extensions_;
    DiffusionListDialog&                      diffusionDialog_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QVBox*                                    pMainLayout_;
    QGroupBox*                                pGroupBox_;
    T_Widgets                                 widgets_;
    //@}
};

#endif // __OrbatAttributesPanel_h_
