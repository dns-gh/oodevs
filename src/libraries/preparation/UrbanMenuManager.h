// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanMenuManager_h_
#define __UrbanMenuManager_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include <boost/noncopyable.hpp>
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class InfrastructureType;
    class StaticModel;
    class UrbanObject_ABC;
}

class UrbanModel;
class QSignalMapper;

// =============================================================================
/** @class  UrbanMenuManager
    @brief  UrbanMenuManager
*/
// Created: JSR 2012-06-07
// =============================================================================
class UrbanMenuManager : public QObject
                       , public tools::Observer_ABC
                       , public kernel::MultipleSelectionObserver< kernel::UrbanObject_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::UrbanObject_ABC >
                       , public kernel::ModesObserver_ABC
                       , private boost::noncopyable
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             UrbanMenuManager( kernel::Controllers& controllers, UrbanModel& model, const kernel::StaticModel& staticModel );
    virtual ~UrbanMenuManager();
    //@}

    //! @name Operations
    //@{
    void Register();
    void Unregister();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateDistrict();
    void OnDelete();
    void OnDeleteInfrastructures();
    void OnCreateInfrastructure( const QString& name );
    void OnApplyTemplate( const QString& name );
    void OnApplyToChildren();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelectionChanged( const T_Elements& elements );
    virtual void NotifyContextMenu( const kernel::UrbanObject_ABC& object, kernel::ContextMenu& menu );
    void CreateInfrastructuresMenu( kernel::ContextMenu& menu ) const;
    void CreateTemplatesMenu( kernel::ContextMenu& menu ) const;
    void DoApplyInfrastructure( kernel::Entity_ABC& object, kernel::InfrastructureType* type ) const;
    void DoApplyToChildren( kernel::Entity_ABC& object ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    UrbanModel& model_;
    const kernel::StaticModel& staticModel_;
    kernel::UrbanObject_ABC* element_;
    QSignalMapper* pInfraMapper_;
    QSignalMapper* pTemplateMapper_;
    T_Elements selected_;
    //@}
};

#endif // __UrbanMenuManager_h_
