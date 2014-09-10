// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTaskPresenter_h_
#define __EventTaskPresenter_h_

#include "EventSubPresenter_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <boost/scoped_ptr.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class EntityResolver_ABC;
}

namespace gui
{
    struct EventTaskViewState;
    template< typename T > class EventView_ABC;

// =============================================================================
/** @class  EventTaskPresenter
    @brief  EventTaskPresenter
*/
// Created: ABR 2013-12-09
// =============================================================================
class EventTaskPresenter : public QObject
                         , public EventSubPresenter_ABC< EventTaskViewState >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventTaskPresenter( EventView_ABC< EventTaskViewState >& view,
                                 kernel::Controllers& controllers,
                                 const kernel::EntityResolver_ABC& model );
    virtual ~EventTaskPresenter();
    //@}

    //! @name Operations
    //@{
    virtual void FillFrom( const Event& event );
    void OnTargetChanged( const kernel::Entity_ABC* entity );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnDescriptionChanged( const QString& label );
    void OnLabelChanged( const QString& label );
    void OnPayloadChanged( const QString& label );
    void OnUrlChanged( const QString& label );
    void OnShowClicked();
    //@}

private:
    //! @name EventSubPresenter_ABC implementation
    //@{
    virtual void Trigger( const gui::Event& event );
    virtual void CommitTo( timeline::Event& event ) const;
    virtual void Clear();
    virtual void Purge();
    //@}

    //! @name Helpers
    //@{
    void SetTasker( const kernel::Entity_ABC* entity );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::EntityResolver_ABC& model_;
    kernel::SafePointer< kernel::Entity_ABC > entity_;
    //@}
};

} //! namespace gui

#endif // __EventTaskPresenter_h_
