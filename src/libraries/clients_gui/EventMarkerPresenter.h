// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __EventMarkerPresenter_h_
#define __EventMarkerPresenter_h_

#include "EventSubPresenter_ABC.h"

namespace gui
{
    struct EventMarkerViewState;
    template< typename T > class EventView_ABC;

// =============================================================================
/** @class  EventMarkerPresenter
    @brief  EventMarkerPresenter
*/
// Created: JSR 2014-10-24
// =============================================================================
class EventMarkerPresenter : public QObject
                           , public EventSubPresenter_ABC< EventMarkerViewState >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventMarkerPresenter( EventView_ABC< EventMarkerViewState >& view, const std::string& uuid );
    virtual ~EventMarkerPresenter();
    //@}

public:
    //! @name Operations
    //@{
    virtual void FillFrom( const Event& event );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnLabelChanged( const QString& label );
    void OnDescriptionChanged( const QString& description );
    void OnResetDrawingsChanged( bool resetDrawings );
    void OnDrawingsPathChanged( const tools::Path& path );
    void OnConfigurationPathChanged( const tools::Path& path );
    //@}

private:
    //! @name EventSubPresenter_ABC implementation
    //@{
    virtual bool ShouldEnableTrigger() const;
    virtual void Trigger( const Event& event );
    virtual void CommitTo( timeline::Event& event ) const;
    virtual void Clear();
    //@}

    const std::string uuid_;
};

} //! namespace gui

#endif // __EventMarkerPresenter_h_
