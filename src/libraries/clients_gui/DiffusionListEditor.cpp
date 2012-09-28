// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DiffusionListEditor.h"
#include "moc_DiffusionListEditor.cpp"
#include "DiffusionListFunctors.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AttributeType.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include <boost/lexical_cast.hpp>

using namespace gui;

namespace
{
    class VerticalLabelWidget : public QWidget
    {
    public:
        VerticalLabelWidget( const QString& text, QWidget* parent ) : QWidget( parent ), text_( text ){}
        virtual ~VerticalLabelWidget(){}

        virtual void paintEvent( QPaintEvent* e )
        {
            QPainter painter( this );
            painter.setPen( Qt::black );
            painter.translate( e->rect().center() );
            painter.rotate( 270 );
            painter.drawText( QPoint( 0, 0 ), text_ );
        }
    private:
        QString text_;
    };
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEditor constructor
// Created: ABR 2012-02-23
// -----------------------------------------------------------------------------
DiffusionListEditor::DiffusionListEditor( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver< kernel::Agent_ABC >& agents, const tools::Resolver< kernel::Formation_ABC >& formations, const kernel::ExtensionTypes& extensions )
    : QWidget( parent )
    , agents_       ( agents )
    , formations_   ( formations )
    , extensions_   ( extensions )
    , extensionName_( "" )
    , tableView_    ( this )
    , dataModel_    ( this )
    , currentTeam_     ( controllers )
{
    // TableView init
    tableView_.setModel( &dataModel_ );
    tableView_.setSortingEnabled( false );
    tableView_.setShowGrid( true );
    tableView_.setAlternatingRowColors( true );
    tableView_.setSelectionMode( QTableView::NoSelection );
    tableView_.setEditTriggers( QTableView::AllEditTriggers );

    // Headers init
    QHeaderView* verticalHV = tableView_.verticalHeader();
    verticalHV->setDefaultSectionSize( 20 );
    verticalHV->setResizeMode( QHeaderView::Fixed );
    QHeaderView* horizontalHV = tableView_.horizontalHeader();
    horizontalHV->setDefaultSectionSize( 20 );
    horizontalHV->setResizeMode( QHeaderView::Fixed );

    // Layout
    QGridLayout* mainLayout = new QGridLayout( this, 2, 2 );
    mainLayout->setRowMinimumHeight( 0, 30 );
    mainLayout->setColumnMinimumWidth( 0, 30 );
    mainLayout->addWidget( new VerticalLabelWidget( tr( "EMITTERS" ), this ), 1, 0 /*, Qt::AlignHCenter | Qt::AlignVCenter*/ );
    mainLayout->addWidget( new QLabel( tr( "RECIPIENTS" ), this ), 0, 1, Qt::AlignHCenter | Qt::AlignVCenter );
    mainLayout->addWidget( &tableView_, 1, 1 );
    // Connections
    connect( verticalHV, SIGNAL( geometriesChanged() ), this, SLOT( OnGeometriesChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEditor destructor
// Created: ABR 2012-02-23
// -----------------------------------------------------------------------------
DiffusionListEditor::~DiffusionListEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEditor::OnGeometriesChanged
// Created: ABR 2012-02-24
// -----------------------------------------------------------------------------
void DiffusionListEditor::OnGeometriesChanged()
{
    unsigned int emittersMaxLength = 0;
    for( QStringList::const_iterator it = emittersHeaders_.begin(); it != emittersHeaders_.end(); ++it )
        emittersMaxLength = std::max< unsigned int >( emittersMaxLength, it->size() );

    unsigned int receiversMaxLength = 0;
    for( QStringList::const_iterator it = receiversHeaders_.begin(); it != receiversHeaders_.end(); ++it )
        receiversMaxLength = std::max< unsigned int >( receiversMaxLength, it->size() );

    QHeaderView* verticalHV = tableView_.verticalHeader();
    QHeaderView* horizontalHV = tableView_.horizontalHeader();
    const double ratio = emittersMaxLength / static_cast< double >( verticalHV->width() );
    horizontalHV->setMinimumHeight( static_cast< int >( receiversMaxLength / ratio ) );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEditor::Purge
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
void DiffusionListEditor::Purge()
{
    emitters_.clear();
    emittersHeaders_.clear();
    receivers_.clear();
    receiversHeaders_.clear();
    dataModel_.clear();
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEditor::Fill
// Created: ABR 2012-02-29
// -----------------------------------------------------------------------------
void DiffusionListEditor::Fill( kernel::SafePointer< kernel::Entity_ABC > currentTeam, const kernel::Entity_ABC* currentAgent /* = 0 */ )
{
    currentTeam_ = currentTeam;
    extensionName_ = extensions_.GetNameByType( kernel::AttributeType::ETypeDiffusionList );
    assert( !extensionName_.empty() );
    // Find emitters
    {
        const DiffusionListEmittersExtractor functor( extensionName_, *currentTeam_, emitters_, emittersHeaders_ );
        if( currentAgent != 0 )
            functor( *currentAgent );
        else
            agents_.Apply( functor );
        assert( emittersHeaders_.count() == emitters_.size() );
    }
    // Find receiver
    {
        const DiffusionListReceiversExtractor functor( extensionName_, *currentTeam_, receivers_, receiversHeaders_ );
        agents_.Apply( functor );
        formations_.Apply( functor );
        assert( receiversHeaders_.count() == receivers_.size() );
    }
    // Fill data
    {
        dataModel_.setColumnCount( receiversHeaders_.count() );
        dataModel_.setHorizontalHeaderLabels( receiversHeaders_ );

        dataModel_.setRowCount( emittersHeaders_.count() );
        dataModel_.setVerticalHeaderLabels( emittersHeaders_ );

        for( unsigned int row = 0; row < emitters_.size(); ++row )
        {
            kernel::DictionaryExtensions* dico = const_cast< kernel::DictionaryExtensions* >( emitters_[ row ]->Retrieve< kernel::DictionaryExtensions >() );
            if( !dico )
                return;
            assert( emitters_[ row ] != 0 );
            unsigned long emitterRowId = emitters_[ row ]->GetId();
            QStringList diffusionList = QStringList::split( DiffusionListData::separator_, dico->GetValue( extensionName_ ).c_str(), false );
            for( unsigned int col = 0; col < receivers_.size(); ++col )
            {
                assert( receivers_[ col ] != 0 );
                QStandardItem* item = new QStandardItem();
                if( emitterRowId == receivers_[ col ]->GetId() )
                {
                    item->setFlags( Qt::ItemIsEnabled );
                    item->setBackground( QBrush( QColor( 200, 200, 200) ) );
                }
                else
                {
                    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
                    item->setCheckState( diffusionList.contains( boost::lexical_cast< std::string >( receivers_[ col ]->GetId() ).c_str() ) ? Qt::Checked : Qt::Unchecked );
                }
                dataModel_.setItem( row, col, item );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEditor::OnClear
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
void DiffusionListEditor::OnClear()
{
    if( !currentTeam_ )
        return;
    for( unsigned int row = 0; row < emitters_.size(); ++row )
    {
        for( unsigned int col = 0; col < receivers_.size(); ++col )
        {
            if( emitters_[ row ]->GetId() != receivers_[ col ]->GetId() )
            {
                QStandardItem* item = dataModel_.item( row, col );
                item->setCheckState( Qt::Unchecked );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEditor::OnGenerate
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
void DiffusionListEditor::OnGenerate()
{
    if( !currentTeam_ )
        return;
    for( unsigned int row = 0; row < emitters_.size(); ++row )
    {
        QStringList generatedDiffusionList;
        const DiffusionListGenerator functor( extensionName_, *currentTeam_, generatedDiffusionList );
        functor( *emitters_[ row ] );
        for( unsigned int col = 0; col < receivers_.size(); ++col )
        {
            if( emitters_[ row ]->GetId() != receivers_[ col ]->GetId() )
            {
                QStandardItem* item = dataModel_.item( row, col );
                item->setCheckState( generatedDiffusionList.contains( boost::lexical_cast< std::string >( receivers_[ col ]->GetId() ).c_str() ) ? Qt::Checked : Qt::Unchecked );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEditor::Commit
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
void DiffusionListEditor::Commit()
{
    if( !currentTeam_ )
        return;
    for( unsigned int row = 0; row < emitters_.size(); ++row )
    {
        std::string diffusionList = "";
        for( unsigned int col = 0; col < receivers_.size(); ++col )
        {
            if( emitters_[ row ]->GetId() != receivers_[ col ]->GetId() )
            {
                QStandardItem* item = dataModel_.item( row, col );
                if( item->checkState() == Qt::Checked )
                {
                    diffusionList += ( diffusionList.empty() ) ? "" : DiffusionListData::separator_;
                    diffusionList += boost::lexical_cast< std::string >( receivers_[ col ]->GetId() );
                }
            }
        }
        kernel::DictionaryExtensions* dico = const_cast< kernel::DictionaryExtensions* >( emitters_[ row ]->Retrieve< kernel::DictionaryExtensions >() );
        if( !dico )
            continue;
        dico->SetValue( extensionName_, diffusionList );
    }
}
