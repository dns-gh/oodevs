// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Wizard_FirstPage_Default.inl $
// $Author: Ape $
// $Modtime: 11/04/05 10:57 $
// $Revision: 3 $
// $Workfile: ADN_Wizard_FirstPage_Default.inl $
//
// *****************************************************************************

#include "ADN_UserData_ListViewItem.h"


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_FirstPage_Default constructor
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
template< typename T >
ADN_Wizard_FirstPage_Default<T>::ADN_Wizard_FirstPage_Default( typename ADN_Wizard_FirstPage_Default<T>::T_ItemVector& existingItemsList, Q3Wizard* pParent, const char* szName, const char* szImageName )
: ADN_Wizard_FirstPage_ABC<T>( pParent, szName, szImageName )
, existingItemsList_         ( existingItemsList )
{
    this->Initialize();

    pParent->addPage( this, "" );
    pParent->setBackEnabled( this, false );
    pParent->setNextEnabled( this, true );
    pParent->setHelpEnabled( this, false );
    pParent->setFinishEnabled( this, false );
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_FirstPage_Default destructor
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
template< typename T >
ADN_Wizard_FirstPage_Default<T>::~ADN_Wizard_FirstPage_Default()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_FirstPage_Default::Initialize
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Wizard_FirstPage_Default< T >::Initialize()
{
    pRightSide_->setSpacing( 10 );
    pRightSide_->setFixedHeight( 200 );

    // The new item's name
    Q3VBox* pNameBox = new Q3VBox( pRightSide_ );
    pNameBox->setSpacing( 5 );

    pNameLabel_ = new QLabel( qApp->translate( "ADN_Wizard", "Name" ), pNameBox );

    pNameEdit_ = new QLineEdit( pNameBox );
    pNameEdit_->setFixedWidth( 350 );
    pNameEdit_->setFocus();

    // Copy or New section
    Q3VBox* pCopyBox = new Q3VBox( pRightSide_ );
    pCopyBox->setSpacing( 5 );

    Q3ButtonGroup* pButtonGroup = new Q3ButtonGroup( 2, Qt::Vertical, pCopyBox );
    pButtonGroup->setMargin( 0 );
    pButtonGroup->setLineWidth( 0 );
    pNewRadioButton_  = new QRadioButton( qApp->translate( "ADN_Wizard", "Create new" ), pButtonGroup );
    pCopyRadioButton_ = new QRadioButton( qApp->translate( "ADN_Wizard", "Create a copy of:" ), pButtonGroup );

    pExistingItemsListView_ = new Q3ListView( pCopyBox );
    pExistingItemsListView_->addColumn( "" );
    pExistingItemsListView_->setMultiSelection( false );
    pExistingItemsListView_->setResizeMode( Q3ListView::LastColumn );

    // Fill the list
    for( IT_ItemVector it = existingItemsList_.begin(); it != existingItemsList_.end(); ++it )
    {
        new ADN_UserData_ListViewItem< T >( pExistingItemsListView_, (*it)->strName_.GetData(), (*it) );
    }

    if( pExistingItemsListView_->firstChild() != 0 )
        pExistingItemsListView_->setSelected( pExistingItemsListView_->firstChild(), true );
    else
        pCopyRadioButton_->setEnabled( false );

    connect( pCopyRadioButton_, SIGNAL( toggled( bool ) ), pExistingItemsListView_, SLOT( setEnabled( bool ) ) );
    pButtonGroup->setButton( 0 );
    pExistingItemsListView_->setEnabled( false );  
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_FirstPage_Default::CreateObject
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_Wizard_FirstPage_Default< T >::CreateObject()
{
    std::string strNewName = pNameEdit_->text().ascii();
    // Check if the name empty.
    QString strErrorTitle( qApp->translate( "ADN_Wizard", "Creation impossible" ) );
    QString strErrorMsg( qApp->translate( "ADN_Wizard", "The provided name is either empty or already in use, please pick another one." ) );

    if( strNewName.empty() )
    {
        QMessageBox::warning( this, strErrorTitle, strErrorMsg, QMessageBox::Ok, Qt::NoButton );
        return 0;
    }

    // Check if the name is not already used.
    for( IT_ItemVector it = existingItemsList_.begin(); it != existingItemsList_.end(); ++it )
    {
        if( (*it)->strName_.GetData() == strNewName )
        {
            QMessageBox::warning( this, strErrorTitle, strErrorMsg, QMessageBox::Ok, Qt::NoButton );
            return 0;
        }
    }

    // Create a completly new item or create a copy of an existing one.
    T* pResult = 0;
    if( pCopyRadioButton_->isOn() )
    {
        Q3ListViewItem* pSelectedItem = pExistingItemsListView_->selectedItem();
        T* pItem = static_cast< ADN_UserData_ListViewItem< T >* >( pSelectedItem )->GetPointer();
        pResult = pItem->CreateCopy();
    }
    else
    {
        pResult = NewT();
    }

    // Set its name and return it.
    pResult->strName_ = strNewName;
    return pResult;
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_FirstPage_Default::SetCaptions
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Wizard_FirstPage_Default< T >::SetCaptions( const char* szPageName, const char* szColumnHeader )
{
    pWizard_->setTitle( this, szPageName );
    pExistingItemsListView_->setColumnText( 0, szColumnHeader );
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_FirstPage_Default::NewT
// Created: APE 2005-02-15
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_Wizard_FirstPage_Default<T>::NewT()
{
    return new T();
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_FirstPage_Default::showEvent
// Created: APE 2005-04-08
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Wizard_FirstPage_Default<T>::showEvent( QShowEvent* pEvent )
{
    pNameEdit_->setFocus();
    QWidget::showEvent( pEvent );
}
