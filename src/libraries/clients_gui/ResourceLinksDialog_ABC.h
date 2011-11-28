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

#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Object_ABC;
    class ResourceNetworkType;
}

namespace gui
{

class TerrainObjectProxy;

// =============================================================================
/** @class  ResourceLinksDialog_ABC
    @brief  ResourceLinksDialog_ABC
*/
// Created: JSR 2010-08-24
// =============================================================================
class ResourceLinksDialog_ABC : public QDockWidget
                              , public tools::Observer_ABC
                              , public tools::SelectionObserver_ABC
                              , public tools::SelectionObserver_Base< TerrainObjectProxy >
                              , public tools::SelectionObserver_Base< kernel::Object_ABC >
                              , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                              , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                              , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ResourceLinksDialog_ABC( QMainWindow* parent, kernel::Controllers& controllers, const tools::StringResolver< kernel::ResourceNetworkType >& resources );
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
    void OnValueChanged( int i, int j );
    void Validate();
    void OnChangeLink( int resourceId );
    void OnCreateNode( int resourceId );
    void GenerateProduction();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoValidate() = 0;
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const TerrainObjectProxy& proxy );
    virtual void Select( const kernel::Object_ABC& element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    virtual void NotifyContextMenu( const kernel::Object_ABC&, kernel::ContextMenu& menu );
    virtual bool DoGenerateProduction() { return false; }
    void Show();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::ResourceNetwork_ABC* selected_;
    Q3ListBoxItem* selectedItem_;
    const tools::StringResolver< kernel::ResourceNetworkType >& resources_;
    const kernel::Object_ABC* linkToChange_;
    bool urban_;
    unsigned int id_;
    kernel::ResourceNetwork_ABC::T_ResourceNodes resourceNodes_;
    Q3VBox* pMainLayout_;
    Q3ListBox* dotationList_;
    Q3GroupBox* groupBox_;
    QSpinBox* production_;
    QPushButton* generateProduction_;
    QSpinBox* consumption_;
    QCheckBox* critical_;
    Q3HBox* stockBox_;
    QSpinBox* maxStock_;
    QSpinBox* stock_;
    Q3Table* table_;
    //@}
};

}

#endif // __ResourceLinksDialog_ABC_h_
