// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_SearchListView_h_
#define __ADN_SearchListView_h_

#include <boost/noncopyable.hpp>
#include "ADN_SearchLineEdit.h"
#include "ADN_UsedByInfos.h"

class ADN_Ref_ABC;

// =============================================================================
/** @class  ADN_SearchListView_ABC
    @brief  ADN_SearchListView_ABC
*/
// Created: ABR 2012-01-19
// =============================================================================
class ADN_SearchListView_ABC : public QWidget
                             , private boost::noncopyable
{
    Q_OBJECT

public:
    explicit ADN_SearchListView_ABC( QWidget* parent = 0 ) : QWidget( parent ) {}
    virtual ~ADN_SearchListView_ABC() {}

public slots:
    virtual void OnApplyFilterList( const ADN_UsedByInfos& /*usedByInfos*/ ) {}
};


// =============================================================================
/** @class  ADN_SearchListView
    @brief  ADN_SearchListView
*/
// Created: ABR 2012-01-19
// =============================================================================
template< typename ListView >
class ADN_SearchListView : public ADN_SearchListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_SearchListView( ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab = -1, QWidget* parent = 0 );
             template< typename FirstParam >
             ADN_SearchListView( FirstParam& first, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab = -1, QWidget* parent = 0 );
             template< typename FirstParam, typename SecondParam >
             ADN_SearchListView( FirstParam& first, SecondParam& second, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab = -1, QWidget* parent = 0 );
    virtual ~ADN_SearchListView();
    //@}

    //! @name Operations
    //@{
    ListView* GetListView() const;
    virtual void OnApplyFilterList( const ADN_UsedByInfos& usedByInfos );
    //@}

private:
    //! @name Member data
    //@{
    ListView*           listView_;
    ADN_SearchLineEdit* lineEdit_;
    int                 subTab_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
template< typename ListView >
ADN_SearchListView< ListView >::ADN_SearchListView( ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab /*= -1*/, QWidget* parent /*= 0*/ )
    : ADN_SearchListView_ABC( parent )
    , listView_( 0 )
    , lineEdit_( 0 )
    , subTab_  ( subTab )
{
    // Layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );

    // Create the list view.
    listView_ = new ListView( this );
    listView_->GetConnector().Connect( &reference );
    listView_->SetItemConnectors( connector );

    // Search box
    lineEdit_ = new ADN_SearchLineEdit( this );
    connect( lineEdit_, SIGNAL( textChanged( const QString& ) ), listView_, SLOT( OnFilterChanged( const QString& ) ) );
    layout->addWidget( lineEdit_);
    layout->addWidget( listView_, 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename FirstParam >
ADN_SearchListView< ListView >::ADN_SearchListView( FirstParam& first, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab /*= -1*/, QWidget* parent /*= 0*/ )
    : ADN_SearchListView_ABC( parent )
    , listView_( 0 )
    , lineEdit_( 0 )
    , subTab_  ( subTab )
{
    // Layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );

    // Create the list view.
    listView_ = new ListView( first, this );
    listView_->GetConnector().Connect( &reference );
    listView_->SetItemConnectors( connector );

    // Search box
    lineEdit_ = new ADN_SearchLineEdit( this );
    connect( lineEdit_, SIGNAL( textChanged( const QString& ) ), listView_, SLOT( OnFilterChanged( const QString& ) ) );
    layout->addWidget( lineEdit_ );
    layout->addWidget( listView_, 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
template< typename ListView >
template< typename FirstParam, typename SecondParam >
ADN_SearchListView< ListView >::ADN_SearchListView( FirstParam& first, SecondParam& second, ADN_Ref_ABC& reference, const T_ConnectorVector& connector, int subTab /*= -1*/, QWidget* parent /*= 0*/ )
    : ADN_SearchListView_ABC( parent )
    , listView_( 0 )
    , lineEdit_( 0 )
    , subTab_  ( subTab )
{
    // Layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );

    // Create the list view.
    listView_ = new ListView( first, second, this );
    listView_->GetConnector().Connect( &reference );
    listView_->SetItemConnectors( connector );

    // Search box
    lineEdit_ = new ADN_SearchLineEdit( this );
    connect( lineEdit_, SIGNAL( textChanged( const QString& ) ), listView_, SLOT( OnFilterChanged( const QString& ) ) );
    layout->addWidget( lineEdit_ );
    layout->addWidget( listView_, 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView destructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
template< typename ListView >
ADN_SearchListView< ListView >::~ADN_SearchListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView::GetListView
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
template< typename ListView >
ListView* ADN_SearchListView< ListView >::GetListView() const
{
    return listView_;
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchListView::OnApplyFilterList
// Created: ABR 2012-01-26
// -----------------------------------------------------------------------------
template< typename ListView >
void ADN_SearchListView< ListView >::OnApplyFilterList( const ADN_UsedByInfos& usedByInfos )
{
    if( usedByInfos.subTargetTab_ != subTab_ )
        return;

    lineEdit_->setReadOnly( true );
    lineEdit_->setText( usedByInfos.usingName_ );
    lineEdit_->setPaletteForegroundColor( Qt::darkGray );
    listView_->OnFilterChanged( usedByInfos.usersList_ );
}

#endif // __ADN_SearchListView_h_
