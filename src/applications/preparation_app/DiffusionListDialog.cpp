// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DiffusionListDialog.h"
#include "moc_DiffusionListDialog.cpp"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AttributeType.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Tools.h"
#include "DiffusionListFunctors.h"
#include "DiffusionListHierarchy.h"
#include "PreparationProfile.h"
#include "preparation/AgentsModel.h"
#include <boost/lexical_cast.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog constructor
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
DiffusionListDialog::DiffusionListDialog( QWidget* parent, Controllers& controllers, const AgentsModel& agents, const ExtensionTypes& extensions, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, const char* name /*= 0*/ )
    : QDialog( parent, name, true )
    , controllers_  ( controllers )
    , extensions_   ( extensions )
    , agents_       ( agents )
    , selected_     ( controllers )
{
    // Init dialog
    setCaption( tr( "Edit diffusion list" ) );
    resize( 500, 700 );
    hide();
    // Main layouts
    QVBoxLayout* mainLayout = new QVBoxLayout( this, 0, 5, "DiffusionListDialog_VBoxLayout_MainLayout" );
    mainLayout->setMargin( 5 );
    // Header
    QGroupBox* header = new QGroupBox( 1, Qt::Horizontal, tr( "Transmitter" ), this, "DiffusionListDialog_GroupBox_Header" );
    transmitterName_ = new QLabel( header, "DiffusionListDialog_Label_TransmitterName" );
    // Editor
    QGroupBox* editor = new QGroupBox( 1, Qt::Horizontal, tr( "Recipients" ), this, "DiffusionListDialog_GroupBox_Editors" );
    list_ = new DiffusionListHierarchy( editor, controllers, factory, PreparationProfile::GetProfile(), icons );
    // Legend
    QGroupBox* legend = new QGroupBox( 1, Qt::Vertical, tr( "Legend" ), editor, "DiffusionListDialog_GroupBox_Legend" );
    AddLegend( tr( "Transmitter:" ), legend, DiffusionListHierarchy::selectedColor_ );
    AddLegend( tr( "Recipients:" ), legend, DiffusionListHierarchy::recipientsColor_ );
    AddLegend( tr( "Potential recipients:" ), legend, DiffusionListHierarchy::potentialsColor_ );
    // Filters
    QGroupBox* filters = new QGroupBox( 1, Qt::Horizontal, tr( "Filters" ), editor, "DiffusionListDialog_GroupBox_Filters" );
    filterLine_ = new QLineEdit( filters, "DiffusionListDialog_LineEdit_Filter" );
    QHBox* filtersBox = new QHBox( filters, "DiffusionListDialog_HBox_FiltersBox" );
    filtersButtons_ = new QButtonGroup( 0, "DiffusionListDialog_ButtonGroup_Filters" );
    filtersButtons_->insert( new QRadioButton( tr( "Transmitter, recipients and potentials recipients" ), filtersBox, "DiffusionListDialog_RadioButton_RecipientsPotentials" ), DiffusionListHierarchy::EFilterRecipientsAndPotentials );
    filtersButtons_->insert( new QRadioButton( tr( "Transmitter and recipients" ), filtersBox, "DiffusionListDialog_RadioButton_Recipients" ), DiffusionListHierarchy::EFilterRecipients );
    filtersButtons_->insert( new QRadioButton( tr( "All" ), filtersBox, "DiffusionListDialog_RadioButton_Full" ), DiffusionListHierarchy::EFilterFull );
    filtersButtons_->setRadioButtonExclusive( true );
    // Results
    QGroupBox* result = new QGroupBox( 1, Qt::Horizontal, tr( "Resulting diffusion list" ), editor, "DiffusionListDialog_GroupBox_Results" );
    text_ = new QLineEdit( result, "DiffusionListDialog_LineEdit_Result" );
    text_->setValidator( new QRegExpValidator( DiffusionListHierarchy::diffusionRegexp_, text_ ) );
    text_->setReadOnly( true );
    // Buttons
    QVBox* buttons = new QVBox( this, "DiffusionListDialog_VBox_Buttons" );
    QHBox* advancedButtons = new QHBox( buttons, "DiffusionListDialog_HBox_AdvancedButtons" );
    QHBox* baseButtons = new QHBox( buttons, "DiffusionListDialog_HBox_BaseButtons" );
    QPushButton* generateButton    = new QPushButton( tr( "Generate" )    , advancedButtons, "DiffusionListDialog_PushButton_Generate" );
    QPushButton* generateAllButton = new QPushButton( tr( "Generate all" ), advancedButtons, "DiffusionListDialog_PushButton_GenerateAll" );
    QPushButton* clearButton       = new QPushButton( tr( "Clear" )       , advancedButtons, "DiffusionListDialog_PushButton_Clear" );
    QPushButton* clearAllButton    = new QPushButton( tr( "Clear all" )   , advancedButtons, "DiffusionListDialog_PushButton_ClearAll" );
    QPushButton* okButton          = new QPushButton( tr( "Ok" )          , baseButtons, "DiffusionListDialog_PushButton_Ok" );
    QPushButton* cancelButton      = new QPushButton( tr( "Cancel" )      , baseButtons, "DiffusionListDialog_PushButton_Cancel" );
    okButton->setDefault( true );
    // Layouts
    mainLayout->add( header );
    mainLayout->add( editor );
    mainLayout->add( buttons );
    // Signals
    connect( okButton         , SIGNAL( clicked() ), SLOT( accept() ) );
    connect( cancelButton     , SIGNAL( clicked() ), SLOT( reject() ) );
    connect( clearButton      , SIGNAL( clicked() ), SLOT( OnClear() ) );
    connect( clearAllButton   , SIGNAL( clicked() ), SLOT( OnClearAll() ) );
    connect( generateButton   , SIGNAL( clicked() ), SLOT( OnGenerate() ) );
    connect( generateAllButton, SIGNAL( clicked() ), SLOT( OnGenerateAll() ) );
    connect( list_            , SIGNAL( ListChanged( const QString& ) ), text_, SLOT( setText( const QString& ) ) );
    connect( filterLine_      , SIGNAL( textChanged( const QString& ) ), list_, SLOT( OnFilterChanged( const QString& ) ) );
    connect( filtersButtons_  , SIGNAL( clicked( int ) ),                list_, SLOT( OnFilterChanged( int ) ));
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog destructor
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
DiffusionListDialog::~DiffusionListDialog()
{
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::AddLegend
// Created: ABR 2011-05-09
// -----------------------------------------------------------------------------
void DiffusionListDialog::AddLegend( const QString label, QWidget* parent, const QColor& color )
{
    new QLabel( label, parent );
    QPixmap* pixmap = new QPixmap( DiffusionListHierarchy::iconSize_, DiffusionListHierarchy::iconSize_ );
    pixmap->fill( color );
    QVBox* box = new QVBox( parent );
    box->setFrameStyle( QFrame::Box + QFrame::Plain );
    box->setLineWidth( 1 );
    box->setBackgroundPixmap( *pixmap );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::accept
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::accept()
{
    selected_ = 0;
    QDialog::accept();
    emit Accepted( text_->text() );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::reject
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::reject()
{
    selected_ = 0;
    QDialog::reject();
    emit Rejected();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::OnClear
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::OnClear()
{
    if( QMessageBox::warning( this, tr( "Cleaning diffusion lists" ), tr( "You are about to erase the selected unit's diffusion list, are you sure?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
        return;
    const DiffusionListClearer functor( extensionName_ );
    functor( *selected_ );
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::OnClearAll
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::OnClearAll()
{
    if( QMessageBox::warning( this, tr( "Cleaning diffusion lists" ), tr( "You are about to erase all diffusion list on the exercise, are you sure?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
        return;
    const DiffusionListClearer functor( extensionName_ );
    agents_.tools::Resolver< Agent_ABC >::Apply( functor );
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::OnGenerate
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::OnGenerate()
{
    if( QMessageBox::warning( this, tr( "Generating diffusion lists" ), tr( "You are about to replace the selected unit's diffusion list, are you sure?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
        return;
    const DiffusionListGenerator functor( extensionName_ );
    functor( *selected_ );
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::OnGenerateAll
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::OnGenerateAll()
{
    if( QMessageBox::warning( this, tr( "Generating diffusion lists" ), tr( "You are about to replace all diffusion list on the exercise, are you sure?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
        return;
    const DiffusionListGenerator functor( extensionName_ );
    agents_.tools::Resolver< Agent_ABC >::Apply( functor );
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::UpdateDisplay
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
void DiffusionListDialog::UpdateDisplay()
{
    const DictionaryExtensions& dico = selected_->Get< DictionaryExtensions >();
    text_->setText( dico.GetValue( extensionName_ ).c_str() );
    list_->UpdateDisplay( text_->text() );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::Purge
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::Purge()
{
    list_->Purge();
    text_->clear();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DiffusionListDialog::Fill
// Created: ABR 2011-04-29
// -----------------------------------------------------------------------------
void DiffusionListDialog::Fill( SafePointer< Entity_ABC > selected, const QString& diffusionList )
{
    if( !selected )
        return;
    selected_ = selected;
    transmitterName_->setText( QString( "%1 [%2]" ).arg( selected_->GetName() ).arg( selected_->GetId() ) );
    if( extensionName_.empty() )
        extensionName_ = extensions_.GetNameByType( AttributeType::ETypeDiffusionList );
    assert( !extensionName_.empty() );
    filtersButtons_->setButton( DiffusionListHierarchy::EFilterRecipientsAndPotentials );
    text_->setText( diffusionList );
    list_->Initialize( selected, diffusionList.ascii() );
}
