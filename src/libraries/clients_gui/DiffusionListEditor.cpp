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
    tableView_.setSelectionMode( RichWidget< QTableView >::NoSelection );
    tableView_.setEditTriggers( RichWidget< QTableView >::AllEditTriggers );

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
    for( DiffusionListData::CIT_Entities it = emitters_.begin(); it != emitters_.end(); ++it )
        emittersMaxLength = std::max< unsigned int >( emittersMaxLength, it->first.size() );

    unsigned int receiversMaxLength = 0;
    for( DiffusionListData::CIT_Entities it = receivers_.begin(); it != receivers_.end(); ++it )
        receiversMaxLength = std::max< unsigned int >( receiversMaxLength, it->first.size() );

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
    receivers_.clear();
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
        const DiffusionListEmittersExtractor functor( extensionName_, *currentTeam_, emitters_ );
        if( currentAgent != 0 )
            functor( *currentAgent );
        else
            agents_.Apply( functor );
    }
    // Find receiver
    {
        const DiffusionListReceiversExtractor functor( extensionName_, *currentTeam_, receivers_ );
        agents_.Apply( functor );
        formations_.Apply( functor );
    }
    // Fill data
    {
        QStringList headerLabels;
        for( DiffusionListData::CIT_Entities it = emitters_.begin(); it != emitters_.end(); ++it )
            headerLabels << it->first;
        dataModel_.setRowCount( static_cast< int >( emitters_.size() ) );
        dataModel_.setVerticalHeaderLabels( headerLabels );

        headerLabels.clear();
        for( DiffusionListData::CIT_Entities it = receivers_.begin(); it != receivers_.end(); ++it )
            headerLabels << it->first;
        dataModel_.setColumnCount( static_cast< int >( receivers_.size() ) );
        dataModel_.setHorizontalHeaderLabels( headerLabels );

        int row = 0;
        for( DiffusionListData::CIT_Entities itEmitter = emitters_.begin(); itEmitter != emitters_.end(); ++itEmitter, ++row )
        {
            assert( itEmitter->second != 0 );
            const kernel::DictionaryExtensions* dico = itEmitter->second->Retrieve< kernel::DictionaryExtensions >();
            if( !dico )
                return;
            unsigned long emitterRowId = itEmitter->second->GetId();
            QStringList diffusionList = QStringList::split( DiffusionListData::separator_, dico->GetValue( extensionName_ ).c_str(), false );

            int column = 0;
            for( DiffusionListData::CIT_Entities itReceiver = receivers_.begin(); itReceiver != receivers_.end(); ++itReceiver, ++column )
            {
                assert( itReceiver->second != 0 );
                QStandardItem* item = new QStandardItem();
                if( emitterRowId == itReceiver->second->GetId() )
                {
                    item->setFlags( Qt::ItemIsEnabled );
                    item->setBackground( QBrush( QColor( 200, 200, 200) ) );
                }
                else
                {
                    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
                    item->setCheckState( diffusionList.contains( boost::lexical_cast< std::string >( itReceiver->second->GetId() ).c_str() ) ? Qt::Checked : Qt::Unchecked );
                }
                dataModel_.setItem( row, column, item );
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

    int row = 0;
    for( DiffusionListData::CIT_Entities itEmitter = emitters_.begin(); itEmitter != emitters_.end(); ++itEmitter, ++row )
    {
        assert( itEmitter->second != 0 );
        int column = 0;
        for( DiffusionListData::CIT_Entities itReceiver = receivers_.begin(); itReceiver != receivers_.end(); ++itReceiver, ++column )
        {
            assert( itReceiver->second != 0 );
            if( itEmitter->second->GetId() != itReceiver->second->GetId() )
                if( QStandardItem* item = dataModel_.item( row, column ) )
                    item->setCheckState( Qt::Unchecked );
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
    int row = 0;
    for( DiffusionListData::CIT_Entities itEmitter = emitters_.begin(); itEmitter != emitters_.end(); ++itEmitter, ++row )
    {
        assert( itEmitter->second != 0 );

        QStringList generatedDiffusionList;
        const DiffusionListGenerator functor( extensionName_, *currentTeam_, generatedDiffusionList );
        functor( *itEmitter->second );

        int column = 0;
        for( DiffusionListData::CIT_Entities itReceiver = receivers_.begin(); itReceiver != receivers_.end(); ++itReceiver, ++column )
        {
            assert( itReceiver->second != 0 );
            if( itEmitter->second->GetId() != itReceiver->second->GetId() )
            {
                QStandardItem* item = dataModel_.item( row, column );
                item->setCheckState( generatedDiffusionList.contains( boost::lexical_cast< std::string >( itReceiver->second->GetId() ).c_str() ) ? Qt::Checked : Qt::Unchecked );
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

    int row = 0;
    for( DiffusionListData::CIT_Entities itEmitter = emitters_.begin(); itEmitter != emitters_.end(); ++itEmitter, ++row )
    {
        assert( itEmitter->second != 0 );
        std::string diffusionList = "";
        int column = 0;
        for( DiffusionListData::CIT_Entities itReceiver = receivers_.begin(); itReceiver != receivers_.end(); ++itReceiver, ++column )
        {
            assert( itReceiver->second != 0 );
            if( itEmitter->second->GetId() != itReceiver->second->GetId() )
            {
                QStandardItem* item = dataModel_.item( row, column );
                if( item->checkState() == Qt::Checked )
                {
                    diffusionList += ( diffusionList.empty() ) ? "" : DiffusionListData::separator_;
                    diffusionList += boost::lexical_cast< std::string >( itReceiver->second->GetId() );
                }
            }
        }
        kernel::DictionaryExtensions* dico = const_cast< kernel::DictionaryExtensions* >( itEmitter->second->Retrieve< kernel::DictionaryExtensions >() );
        if( !dico )
            continue;
        dico->SetValue( extensionName_, diffusionList );
    }
}
