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

#include "clients_gui/EventView_ABC.h"

namespace gui
{
    class EventPresenter;
    class Viewport_ABC;
}

// =============================================================================
/** @class  EventWidget_ABC
    @brief  EventWidget_ABC
*/
// Created: ABR 2013-05-30
// =============================================================================
template< typename ViewState >
class EventWidget_ABC : public QWidget
                      , public gui::EventView_ABC< ViewState >
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

    //! @name EventView_ABC empty implementation
    //@{
    virtual void Purge() {}
    virtual void Build( const ViewState& /*state*/ ) {}
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

typedef EventWidget_ABC< gui::EventViewState >                 EventDefaultWidget_ABC;
typedef EventWidget_ABC< gui::EventMagicViewState >            EventMagicWidget_ABC;
typedef EventWidget_ABC< gui::EventOrderViewState >            EventOrderWidget_ABC;
typedef EventWidget_ABC< gui::EventTaskViewState >             EventTaskWidget_ABC;

#endif // __EventWidget_ABC_h_
