// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __SearchListView_h_
#define __SearchListView_h_

#include <boost/noncopyable.hpp>
#include "SearchLineEdit.h"
#include "resources.h"

namespace gui
{

// =============================================================================
/** @class  SearchListView
    @brief  SearchListView
*/
// Created: ABR 2012-03-27
// =============================================================================
template< typename ListView >
class SearchListView : public QWidget
                     , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SearchListView( QWidget* parent );
             template< typename _1 >
             SearchListView( QWidget* parent, _1& first );
             template< typename _1, typename _2 >
             SearchListView( QWidget* parent, _1& first, _2& second );
             template< typename _1, typename _2, typename _3 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third );
             template< typename _1, typename _2, typename _3, typename _4 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth );
             template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6, typename _7 >
             SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth, _7& seventh );
    virtual ~SearchListView();
    //@}

    //! @name Operations
    //@{
    ListView* GetListView() const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateGUI();
    //@}

private:
    //! @name Member data
    //@{
    SearchLineEdit* searchLine_;
    ListView*       listView_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: SearchListView constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
SearchListView< ListView >::SearchListView( QWidget* parent )
    : QWidget( parent )
    , searchLine_( 0 )
    , listView_  ( 0 )
{
    listView_ = new ListView( this );
    CreateGUI();
}

// -----------------------------------------------------------------------------
// Name: SearchListView constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename _1 >
SearchListView< ListView >::SearchListView( QWidget* parent, _1& first )
    : QWidget( parent )
    , searchLine_( 0 )
    , listView_  ( 0 )
{
    listView_ = new ListView( this, first );
    CreateGUI();
}

// -----------------------------------------------------------------------------
// Name: SearchListView constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename _1, typename _2 >
SearchListView< ListView >::SearchListView( QWidget* parent, _1& first, _2& second )
    : QWidget( parent )
    , searchLine_( 0 )
    , listView_  ( 0 )
{
    listView_ = new ListView( this, first, second );
    CreateGUI();
}

// -----------------------------------------------------------------------------
// Name: SearchListView constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename _1, typename _2, typename _3 >
SearchListView< ListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third )
    : QWidget( parent )
    , searchLine_( 0 )
    , listView_  ( 0 )
{
    listView_ = new ListView( this, first, second, third );
    CreateGUI();
}

// -----------------------------------------------------------------------------
// Name: SearchListView constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename _1, typename _2, typename _3, typename _4 >
SearchListView< ListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth )
    : QWidget( parent )
    , searchLine_( 0 )
    , listView_  ( 0 )
{
    listView_ = new ListView( this, first, second, third, fourth );
    CreateGUI();
}

// -----------------------------------------------------------------------------
// Name: SearchListView constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename _1, typename _2, typename _3, typename _4, typename _5 >
SearchListView< ListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth )
    : QWidget( parent )
    , searchLine_( 0 )
    , listView_  ( 0 )
{
    listView_ = new ListView( this, first, second, third, fourth, fifth );
    CreateGUI();
}

// -----------------------------------------------------------------------------
// Name: SearchListView constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6 >
SearchListView< ListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth )
    : QWidget( parent )
    , searchLine_( 0 )
    , listView_  ( 0 )
{
    listView_ = new ListView( this, first, second, third, fourth, fifth, sixth );
    CreateGUI();
}

// -----------------------------------------------------------------------------
// Name: SearchListView constructor
// Created: ABR 2012-03-28
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename _1, typename _2, typename _3, typename _4, typename _5, typename _6, typename _7 >
SearchListView< ListView >::SearchListView( QWidget* parent, _1& first, _2& second, _3& third, _4& fourth, _5& fifth, _6& sixth, _7& seventh )
    : QWidget( parent )
    , searchLine_( 0 )
    , listView_  ( 0 )
{
    listView_ = new ListView( this, first, second, third, fourth, fifth, sixth, seventh );
    CreateGUI();
}

// -----------------------------------------------------------------------------
// Name: SearchListView destructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
SearchListView< ListView >::~SearchListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SearchListView::GetListView
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
inline
ListView* SearchListView< ListView >::GetListView() const
{
    return listView_;
}

// -----------------------------------------------------------------------------
// Name: SearchListView::CreateGUI
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
template< typename ListView >
inline
void SearchListView< ListView >::CreateGUI()
{
    assert( listView_ != 0 );

    // Search box
    searchLine_ = new SearchLineEdit( this );

    // Next Button
    QPushButton* next = new QPushButton( QIcon( MAKE_ICON( search ) ), "", this );
    next->setAccel( Qt::Key_F3 );
    next->setMaximumWidth( searchLine_->height() );
    next->setMaximumHeight( searchLine_->height() );

    // Connection
    connect( searchLine_, SIGNAL( textChanged( const QString& ) ), listView_, SLOT( SearchAndSelect( const QString& ) ) );
    connect( searchLine_, SIGNAL( returnPressed() ),               listView_, SLOT( SearchAndSelectNext() ) );
    connect( next,      SIGNAL( pressed() ),                     listView_, SLOT( SearchAndSelectNext() ) );

    // Layout
    QGridLayout* layout = new QGridLayout( this, 2, 2, 5 );
    layout->setMargin( 2 );
    layout->addWidget( searchLine_, 0, 0 );
    layout->addWidget( next, 0, 1 );
    layout->addWidget( listView_, 1, 0, 1, 2 );
}

} //! namespace gui

#endif // __SearchListView_h_
