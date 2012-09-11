// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ChangeAutomatTypeDialog.h"
#include "moc_ChangeAutomatTypeDialog.cpp"
#include "ModelBuilder.h"
#include "clients_gui/UnitListView.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"

namespace
{
    class LocalUnitListView : public gui::UnitListView
    {
    public:
        //! @name Constructors/Destructor
        //@{
        LocalUnitListView( QWidget* parent, kernel::Controllers& controllers, const kernel::AgentTypes& list, gui::ItemFactory_ABC& factory )
            : gui::UnitListView( parent, controllers, list, factory )
        {
            SetSorting( "" );
            Q3ListViewItemIterator it( this );
            while( *it )
            {
                if( gui::ValuedListItem* item = dynamic_cast< gui::ValuedListItem* >( *it ) )
                    item->setSelectable( item->IsA< const kernel::AutomatType >() );
                ++it;
            }
        }

        virtual ~LocalUnitListView()
        {
            // NOTHING
        }
        //@}

        //! @name Operations
        //@{
        virtual QSize sizeHint() const
        {
            return QSize( 300, 600 );
        }
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: ChangeAutomatTypeDialog constructor
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
ChangeAutomatTypeDialog::ChangeAutomatTypeDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::AgentTypes& list, gui::ModelObserver_ABC& builder, gui::ItemFactory_ABC& factory, kernel::Entity_ABC& entity, const std::string& typeName )
    : QDialog( parent )
    , controllers_( controllers )
    , builder_    ( builder )
    , entity_     ( entity )
{
    setModal( true );
    setCaption( tr( "Replace by a new automat" ) );

    QLabel* label1 = new QLabel( tr( "Select new automat type for ") + entity_.GetName() );
    QLabel* label2 = new QLabel( tr( "Current type: ") + typeName.c_str() );
    list_ = new ::LocalUnitListView( 0, controllers, list, factory );
    okBtn_ = new QPushButton( tr( "Ok" ) );
    okBtn_->setEnabled( false );
    okBtn_->setDefault( true );
    QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ) );

    connect( list_ , SIGNAL( clicked( Q3ListViewItem* ) ), SLOT( SelectionChanged() ) );
    connect( okBtn_, SIGNAL( clicked() ), SLOT( OnOk() ) );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( reject() ) );

    QGridLayout* layout = new QGridLayout( this );
    layout->setSpacing( 10 );
    layout->addWidget( label1, 0, 0, 1, 2 );
    layout->addWidget( label2, 1, 0, 1, 2 );
    layout->addWidget( list_, 2, 0, 1, 2 );
    layout->setRowStretch( 2, 1 );
    layout->addWidget( okBtn_, 3, 0 );
    layout->addWidget( cancelBtn, 3, 1 );

    exec();
}

// -----------------------------------------------------------------------------
// Name: ChangeAutomatTypeDialog destructor
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
ChangeAutomatTypeDialog::~ChangeAutomatTypeDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChangeAutomatTypeDialog::SelectionChanged
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
void ChangeAutomatTypeDialog::SelectionChanged()
{
    okBtn_->setEnabled( list_->selectedItem() != 0 );
}

// -----------------------------------------------------------------------------
// Name: ChangeAutomatTypeDialog::OnOk
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
void ChangeAutomatTypeDialog::OnOk()
{
    gui::ValuedListItem* item = dynamic_cast< gui::ValuedListItem* >( list_->selectedItem() );
    if( item && item->IsA< const kernel::AutomatType >() )
        if( const kernel::AutomatType* type = item->GetValueNoCheck< const kernel::AutomatType >() )
            if( kernel::Automat_ABC* result = builder_.ReplaceAutomat( entity_, *type ) )
                result->Select( controllers_.actions_ );
    accept();
}
