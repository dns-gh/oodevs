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

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Controllers;
    class DotationType;
    class Object_ABC;
    class Profile_ABC;
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
class ResourceLinksDialog_ABC : public QDialog
                              , public tools::Observer_ABC
                              , public kernel::ContextMenuObserver_ABC< TerrainObjectProxy >
                              , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ResourceLinksDialog_ABC( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver, const kernel::Profile_ABC& profile );
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
    void OnStockChanged( int value );
    void OnValueChanged( int i, int j );
    void Validate();
    void Reject();
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
    virtual void NotifyContextMenu( const TerrainObjectProxy& proxy, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu );
    virtual void showEvent( QShowEvent* );
    virtual QSize sizeHint();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const tools::Resolver_ABC< kernel::DotationType >& dotationResolver_;
    const kernel::Profile_ABC& profile_;
    const kernel::ResourceNetwork_ABC* selected_;
    bool urban_;
    unsigned int id_;
    kernel::ResourceNetwork_ABC::ResourceNodes resourceNodes_;
    QListView* dotationList_;
    QGroupBox* groupBox_;
    QSpinBox* production_;
    QSpinBox* consumption_;
    QCheckBox* critical_;
    QSpinBox* stock_;
    QTable* table_;
    //@}
};

}

#endif // __ResourceLinksDialog_ABC_h_
