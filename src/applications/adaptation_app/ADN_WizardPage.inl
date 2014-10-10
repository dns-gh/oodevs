// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_WizardPage_inl_
#define __ADN_WizardPage_inl_

#include "clients_kernel/VariantPointer.h"

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage constructor
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
ADN_WizardPage< T >::ADN_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* parent /* = 0 */ )
    : QWizardPage( parent )
    , existingItems_( existingItems )
    , element_      ( 0 )
    , nameEdit_     ( 0 )
{
    setTitle( pageTitle );
    setPixmap( QWizard::WatermarkPixmap, "resources/images/gui/wizard.png" );
    setPixmap( QWizard::LogoPixmap, "resources/images/gui/logo32x32.png" );
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage destructor
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
ADN_WizardPage< T >::~ADN_WizardPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::Build
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
void ADN_WizardPage< T >::Build()
{
    // Name
    nameLabel_ = new QLabel();
    QWidget* nameEditor = CreateNameField();
    assert( nameEditor != 0 );

    // Optional field
    QWidget* optionalWidget = CreateOptionalField();

    // Buttons
    QGroupBox* buttonGroupBox = new gui::RichGroupBox( "button" );
    QVBoxLayout* buttonLayout = new QVBoxLayout( buttonGroupBox );
    buttonGroup_ = new QButtonGroup( buttonGroupBox );
    QRadioButton* newRadioButton = new QRadioButton();
    QRadioButton* copyRadioButton = new QRadioButton();
    newRadioButton->setChecked( true );
    buttonGroup_->addButton( newRadioButton, eNew );
    buttonGroup_->addButton( copyRadioButton, eCopy );
    buttonLayout->addWidget( newRadioButton );
    buttonLayout->addWidget( copyRadioButton );

    // Table
    view_ = new QListView();
    model_ = new QStandardItemModel( view_ );
    model_->setColumnCount( 1 );

    proxy_ = new QSortFilterProxyModel( view_ );
    proxy_->setSourceModel( model_ );
    proxy_->setSortRole( Qt::EditRole );

    view_->setModel( proxy_ );
    view_->setSelectionBehavior( QAbstractItemView::SelectRows );
    view_->setSelectionMode( QAbstractItemView::SingleSelection );
    view_->setEnabled( false );

    int row = 0;
    for( auto it = existingItems_.begin(); it != existingItems_.end(); ++it, ++row )
    {
        QStandardItem* item = new QStandardItem( ( *it )->strName_.GetData().c_str() );
        QVariant variant;
        variant.setValue( kernel::VariantPointer( *it ) );
        item->setData( variant, Qt::UserRole );
        model_->setItem( row, 0, item );
    }
    proxy_->sort( 0 );

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget( nameLabel_ );
    mainLayout->addWidget( nameEditor );
    if( optionalWidget != 0 )
        mainLayout->addWidget( optionalWidget );

    mainLayout->addWidget( buttonGroupBox );
    mainLayout->addWidget( view_, 1 );

    setLayout( mainLayout );
    connect( copyRadioButton, SIGNAL( toggled( bool ) ), view_, SLOT( setEnabled( bool ) ) );
    InitializeTranslation();
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::CreateObject
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_WizardPage< T >::CreateObject()
{
    element_ = 0;
    std::string strNewName = GetName();

    // Check if the name empty.
    if( strNewName.empty() )
    {
        QMessageBox::warning( this, errorTitle_, errorMsg_, QMessageBox::Ok, Qt::NoButton );
        return 0;
    }

    // Check if the name is not already used.
    QList< QStandardItem* > items = model_->findItems( strNewName.c_str(), Qt::MatchExactly );
    if( !items.isEmpty() )
    {
        QMessageBox::warning( this, errorTitle_, errorMsg_, QMessageBox::Ok, Qt::NoButton );
        return 0;
    }

    // Create a new item or create a copy of an existing one.
    if( static_cast< QRadioButton* >( buttonGroup_->button( eCopy ) )->isOn() )
    {
        QItemSelection selection = proxy_->mapSelectionToSource( view_->selectionModel()->selection() );
        QModelIndexList indexes = selection.indexes();
        if( indexes.size() == 0 )
        {
            QMessageBox::warning( this, errorTitle_, noSelectionMsg_, QMessageBox::Ok, Qt::NoButton );
            return 0;
        }
        const T* objectToCopy = static_cast< const T* >( indexes[ 0 ].data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ );
        assert( objectToCopy );
        element_ = const_cast< T* >( objectToCopy )->CreateCopy();
    }
    else
    {
        element_ = NewT();
    }

    // Set name and options
    if( element_ )
    {
        if( !element_->strName_.GetData().empty() )
            throw MASA_EXCEPTION( "Cloned elements should have an empty name before setting the new one" );
        element_->strName_ = strNewName;
    }
    ApplyOptions();
    return element_;
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::SetCopyOf
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
void ADN_WizardPage< T >::SetCopyOf( const std::string& targetName )
{
    copyName_ = targetName;
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::CreateNameField
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
QWidget* ADN_WizardPage< T >::CreateNameField()
{
    nameEdit_ = new QLineEdit();
    return nameEdit_;
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::GetName
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
std::string ADN_WizardPage< T >::GetName()
{
    if( nameEdit_ != 0 )
        return nameEdit_->text().toStdString();
    return "";
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::CreateOptionalField
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
QWidget* ADN_WizardPage< T >::CreateOptionalField()
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::ApplyOptions
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
void ADN_WizardPage< T >::ApplyOptions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::showEvent
// Created: ABR 2012-07-27
// -----------------------------------------------------------------------------
template< typename T >
void ADN_WizardPage< T >::showEvent( QShowEvent* pEvent )
{
    if( nameEdit_ )
        nameEdit_->setFocus();
    if( !copyName_.empty() )
    {
        static_cast< QRadioButton* >( buttonGroup_->button( eCopy ) )->setChecked( true );

        QList< QStandardItem* > items = model_->findItems( copyName_.c_str(), Qt::MatchExactly );
        assert( items.size() == 1 && items[ 0 ] != 0 );

        QModelIndex proxyIndex = proxy_->mapFromSource( model_->indexFromItem( items[ 0 ] ) );
        view_->scrollTo( proxyIndex, QAbstractItemView::PositionAtCenter );
        view_->selectionModel()->select( proxyIndex, QItemSelectionModel::Select );
    }
    QWizardPage::showEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::validatePage
// Created: ABR 2012-07-30
// -----------------------------------------------------------------------------
template< typename T >
bool ADN_WizardPage< T >::validatePage()
{
    return CreateObject() != 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::GetCreatedObject
// Created: ABR 2012-07-30
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_WizardPage< T >::GetCreatedObject() const
{
    return element_;
}

// -----------------------------------------------------------------------------
// Name: ADN_WizardPage::NewT
// Created: ABR 2012-07-31
// -----------------------------------------------------------------------------
template< typename T >
T* ADN_WizardPage< T >::NewT()
{
    return new T();
}

#endif // __ADN_WizardPage_inl_
