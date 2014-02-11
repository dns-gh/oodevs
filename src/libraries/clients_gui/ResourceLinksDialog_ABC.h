// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceLinksDialog_ABC_h_
#define __ResourceLinksDialog_ABC_h_

#include "RichDockWidget.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Model_ABC;
    class Object_ABC;
    class ResourceNetworkType;
    class UrbanObject_ABC;
}

namespace gui
{
    class ResourceNetwork_ABC;
    struct ResourceNode;
    template< typename T > class RichWidget;
    class RichCheckBox;
    class RichGroupBox;
    class RichPushButton;
    class RichSpinBox;

// =============================================================================
/** @class  ResourceLinksDialog_ABC
    @brief  ResourceLinksDialog_ABC
*/
// Created: JSR 2010-08-24
// =============================================================================
class ResourceLinksDialog_ABC : public RichDockWidget
                              , public kernel::MultipleSelectionObserver_ABC
                              , public kernel::MultipleSelectionObserver_Base< kernel::UrbanObject_ABC >
                              , public kernel::MultipleSelectionObserver_Base< kernel::Object_ABC >
                              , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                              , public tools::ElementObserver_ABC< gui::ResourceNetwork_ABC >
                              , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                              , public kernel::ContextMenuObserver_ABC< kernel::UrbanObject_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ResourceLinksDialog_ABC( QMainWindow* parent, const kernel::Model_ABC& model, kernel::Controllers& controllers, const tools::StringResolver< kernel::ResourceNetworkType >& resources );
    virtual ~ResourceLinksDialog_ABC();
    //@}

private slots:
    //! @name Slots
    //@{
    void Update();
    void OnActivationChanged( bool on );
    void OnProductionChanged( int value );
    void OnConsumptionChanged( int value );
    void OnCriticalChanged( bool on );
    void OnMaxStockChanged( int value );
    void OnStockChanged( int value );
    void OnValueChanged();
    void Validate();
    void Cancel();
    void OnChangeLink( int resourceId );
    void OnCreateNode( int resourceId );
    void OnRemoveNode( int resourceId );
    void GenerateProduction();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoValidate( kernel::Entity_ABC* element = 0 ) = 0;
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void MultipleSelect( const std::vector< const kernel::UrbanObject_ABC* >& elements );
    virtual void MultipleSelect( const std::vector< const kernel::Object_ABC* >& elements );

    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    virtual void NotifyUpdated( const gui::ResourceNetwork_ABC& element );
    virtual void NotifyContextMenu( const kernel::Object_ABC&, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::UrbanObject_ABC&, kernel::ContextMenu& menu );
    virtual void SetReadOnly( bool readOnly ) const;
    virtual bool DoGenerateProduction() { return false; }

    void Show();
    void DoNotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    template< class T >
    void DoMultipleSelect( const std::vector< const T* >& elements );
    bool IsDataUpdateEnabled();
    bool RefreshListNeeded() const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Model_ABC& model_;
    std::vector< kernel::Entity_ABC* > selected_;
    QListWidgetItem* selectedItem_;
    bool blockSlot_;
    const tools::StringResolver< kernel::ResourceNetworkType >& resources_;
    kernel::Entity_ABC* sourceNode_;
    unsigned int id_;
    std::map< std::string, ::gui::ResourceNode > resourceNodes_;
    Q3VBox* pMainLayout_;
    RichWidget< QListWidget >* dotationList_;
    RichGroupBox* groupBox_;
    RichSpinBox* production_;
    RichPushButton* generateProduction_;
    RichSpinBox* consumption_;
    RichCheckBox* critical_;
    Q3HBox* stockBox_;
    RichSpinBox* maxStock_;
    RichSpinBox* stock_;
    RichWidget< QTableWidget >* table_;
    RichPushButton* okButton_;
    RichPushButton* cancelButton_;
    //@}
};

}

#endif // __ResourceLinksDialog_ABC_h_
