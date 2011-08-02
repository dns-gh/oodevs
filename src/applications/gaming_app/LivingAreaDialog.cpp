// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LivingAreaDialog.h"
#include "moc_LivingAreaDialog.cpp"
#include "clients_kernel/LivingArea_ABC.h"
#include "clients_kernel/controllers.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/ItemFactory_ABC.h"
#include "reports/RcEntityResolver_ABC.h"
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>


namespace
{
    void Initialize( Q3Table& table, const std::vector< std::string >& cols )
    {
        table.setSorting( true );
        for( unsigned int i = 0; i < cols.size(); ++i )
        {
            table.insertColumns( i );
            table.horizontalHeader()->setLabel( i, cols.at( i ).c_str() );
            table.sortColumn( i, true, true );
        }
        table.setSelectionMode( Q3Table::NoSelection );
        table.setReadOnly( true );
        table.setShowGrid( false );
    }

    class LivingAreaTable : public Q3Table
    {
    public:
        explicit LivingAreaTable( QWidget* parent )
            : Q3Table( parent )
            , sortColumn_( -1 )
        {
            // NOTHING
        }
        virtual void sortColumn( int col, bool ascending = TRUE, bool /*wholeRows*/ = FALSE )
        {
            if( col == -1 )
                col = sortColumn_;
            sortColumn_ = col;
            Q3Table::sortColumn( col, ascending, true );
        }
    private:
        int sortColumn_;
    };

    class IntegerTableItem : public Q3TableItem
    {
    public:
        IntegerTableItem( Q3Table* table, Q3TableItem::EditType et, const QString& text )
            : Q3TableItem( table, et, text )
        {
            // NOTHING
        }
        virtual QString key() const
        {
            return QString( "%1" ).arg( text().toInt(), 15 );
        }
    };

    class FloatTableItem : public Q3TableItem
    {
    public:
        FloatTableItem( Q3Table* table, Q3TableItem::EditType et, const QString& text )
            : Q3TableItem( table, et, text )
        {
            // NOTHING
        }
        virtual QString key() const
        {
            return QString( "%1" ).arg( text().toFloat(), 15 );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: LivingAreaDialog constructor
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
LivingAreaDialog::LivingAreaDialog( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const RcEntityResolver_ABC& rcResolver )
    : QDialog( pParent, tools::translate( "LivingAreaDialog", "Living area" ) )
    , controllers_( controllers )
    , factory_    ( factory )
    , rcResolver_ ( rcResolver )
    , selected_   ( controllers )
    , table_      ( 0 )
{
    resize( 650, 400 );
    Q3VBoxLayout* mainLayout = new Q3VBoxLayout( this, 0, 5 );
    table_ = new LivingAreaTable( this );
    Initialize( *table_, boost::assign::list_of( tools::translate( "LivingAreaDialog", "Name" ).ascii() )( tools::translate( "LivingAreaDialog", "Resident" ).ascii() )
                                               ( tools::translate( "LivingAreaDialog", "Angriness" ).ascii() )( tools::translate( "LivingAreaDialog", "Alerted" ).ascii() )
                                               ( tools::translate( "LivingAreaDialog", "Confined" ).ascii() )( tools::translate( "LivingAreaDialog", "Evacuated" ).ascii() ) );
    mainLayout->add( table_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaDialog destructor
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
LivingAreaDialog::~LivingAreaDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaDialog::NotifyContextMenu
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
void LivingAreaDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = &entity;
    menu.InsertItem( "Update", tools::translate( "LivingAreaDialog", "Living area" ), this, SLOT( Show() ) );
}

// -----------------------------------------------------------------------------
// Name: LivingAreaDialog::NotifySelected
// Created: LGY 2011-05-04
// -----------------------------------------------------------------------------
void LivingAreaDialog::NotifySelected( const kernel::Inhabitant_ABC* entity )
{
    if( entity && isShown() && selected_ != entity )
    {
        selected_ = entity;
        Update();
        table_->sortColumn( -1 );
        table_->sortColumn( -1, false );
    }
}

// -----------------------------------------------------------------------------
// Name: LivingAreaDialog::Show
// Created: LGY 2011-04-29
// -----------------------------------------------------------------------------
void LivingAreaDialog::Show()
{
    if( selected_ && ( !current_ || current_ != selected_->GetId() ) )
        Update();
    show();
}

// -----------------------------------------------------------------------------
// Name: LivingAreaDialog::Update
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void LivingAreaDialog::Update()
{
    setCaption( tools::translate( "LivingAreaDialog", "Living area: %1" ).arg( selected_->GetName() ) );
    table_->setNumRows( 0 );
    rows_.clear();
    if( const kernel::LivingArea_ABC* area = selected_->Retrieve< kernel::LivingArea_ABC >() )
    {
        area->Accept( *this );
        current_ = selected_->GetId();
    }
}

namespace
{
    QString Convert( bool value )
    {
        return value ?  tools::translate( "LivingAreaDialog", "Yes" ) : tools::translate( "LivingAreaDialog", "No" );
    }
}

// -----------------------------------------------------------------------------
// Name: LivingAreaDialog::Visit
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void LivingAreaDialog::Visit( unsigned int id, unsigned int resident, bool alerted, float angriness, bool confined, bool evacuated )
{
    T_Rows::const_iterator it = rows_.find( id );
    if( it == rows_.end() )
    {
        const unsigned int rows = table_->numRows();
        table_->insertRows( rows );
        gui::RichLabel* label = factory_.CreateLabel( rcResolver_.CreateLink( gui::TerrainObjectProxy::typeName_, id ), table_ );
        table_->setCellWidget( rows, 0, label );
        table_->setItem( rows, 1, new IntegerTableItem( table_, Q3TableItem::Never, boost::lexical_cast< std::string >( resident ).c_str() ) );
        table_->setItem( rows, 2, new FloatTableItem( table_, Q3TableItem::Never, boost::lexical_cast< std::string >( angriness ).c_str() ) );
        table_->setItem( rows, 3, new Q3TableItem( table_, Q3TableItem::Never, Convert( alerted ) ) );
        table_->setItem( rows, 4, new Q3TableItem( table_, Q3TableItem::Never, Convert( confined ) ) );
        table_->setItem( rows, 5, new Q3TableItem( table_, Q3TableItem::Never, Convert( evacuated ) ) );
        rows_[ id ] = rows;
    }
    else
    {
        table_->setText( it->second, 1, boost::lexical_cast< std::string >( resident ).c_str() );
        table_->setText( it->second, 2, boost::lexical_cast< std::string >( angriness ).c_str() );
        table_->setText( it->second, 3, Convert( alerted ) );
        table_->setText( it->second, 4, Convert( confined ) );
        table_->setText( it->second, 5, Convert( evacuated ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LivingAreaDialog::NotifyUpdated
// Created: LGY 2011-05-04
// -----------------------------------------------------------------------------
void LivingAreaDialog::NotifyUpdated( const kernel::LivingArea_ABC& result )
{
    if( selected_ && selected_->GetId() == result.GetId() )
    {
        Update();
        table_->sortColumn( -1 );
        table_->sortColumn( -1, false );
    }
}
