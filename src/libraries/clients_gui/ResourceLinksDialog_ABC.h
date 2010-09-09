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
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver.h"
#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class DotationType;
    class Entity_ABC;
    class Object_ABC;
    class Profile_ABC;
}

namespace gui
{

class TerrainObjectProxy;
class ResourceItem;

// =============================================================================
/** @class  ResourceLinksDialog_ABC
    @brief  ResourceLinksDialog_ABC
*/
// Created: JSR 2010-08-24
// =============================================================================
class ResourceLinksDialog_ABC : public QDockWindow
                              , public tools::Observer_ABC
                              , public tools::SelectionObserver_ABC
                              , public tools::SelectionObserver_Base< TerrainObjectProxy >
                              , public tools::SelectionObserver_Base< kernel::Object_ABC >
                              , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ResourceLinksDialog_ABC( QMainWindow* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const kernel::Profile_ABC& profile );
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceLinksDialog_ABC( const ResourceLinksDialog_ABC& );            //!< Copy constructor
    ResourceLinksDialog_ABC& operator=( const ResourceLinksDialog_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoValidate() = 0;
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const TerrainObjectProxy& proxy );
    virtual void Select( const kernel::Object_ABC& element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    void Show();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    const kernel::Profile_ABC& profile_;
    const kernel::ResourceNetwork_ABC* selected_;
    ResourceItem* selectedItem_;
    bool urban_;
    unsigned int id_;
    kernel::ResourceNetwork_ABC::ResourceNodes resourceNodes_;
    QVBox* pMainLayout_;
    QListView* dotationList_;
    QGroupBox* groupBox_;
    QSpinBox* production_;
    QSpinBox* consumption_;
    QCheckBox* critical_;
    QHBox* stockBox_;
    QSpinBox* maxStock_;
    QSpinBox* stock_;
    QTable* table_;
    //@}
};

}

#endif // __ResourceLinksDialog_ABC_h_
