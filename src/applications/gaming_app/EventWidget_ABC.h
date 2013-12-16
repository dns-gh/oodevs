// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventWidget_ABC_h_
#define __EventWidget_ABC_h_

namespace gui
{
    class EventPresenter;
    struct EventViewState;
    class Viewport_ABC;
}

// =============================================================================
/** @class  EventWidget_ABC
    @brief  EventWidget_ABC
*/
// Created: ABR 2013-05-30
// =============================================================================
template< typename T > // T must inherit EventView_ABC
class EventWidget_ABC : public QWidget
                      , public T
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EventWidget_ABC( gui::EventPresenter& presenter, QWidget* parent = 0 )
        : QWidget( parent )
        , presenter_( presenter )
        , mainLayout_( 0 )
    {
        mainLayout_ = new QVBoxLayout( this );
        mainLayout_->setMargin( 0 );
        mainLayout_->setSpacing( 0 );
    }
    //@}

    //! @name Operations
    //@{
    virtual void Purge() {}
    virtual void Build( const gui::EventViewState& /*state*/ ) {}
    virtual void Update( const gui::EventViewState& /*state*/ ) {}

    virtual void BlockSignals( bool /*blocked*/ ) {}
    virtual void Draw( gui::Viewport_ABC& /*viewport*/ ) {}
    //@}

protected:
    //! @name Member data
    //@{
    QVBoxLayout* mainLayout_;
    gui::EventPresenter& presenter_;
    //@}
};

#endif // __EventWidget_ABC_h_
