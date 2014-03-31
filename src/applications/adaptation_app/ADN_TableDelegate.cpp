// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_TableDelegate.h"

#include "ADN_ApplyButton.h"
#include "ADN_ComboBox_Enum.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_EditLine.h"
#include "ADN_StandardItem.h"
#include "ADN_Table.h"
#include "ADN_TimeEdit.h"
#include "ADN_TimeField.h"
#include "ADN_ColorSelector.h"
#include "clients_gui/Roles.h"
#include "clients_kernel/VariantPointer.h"

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate constructor
// Created: ABR 2012-10-22
// -----------------------------------------------------------------------------
ADN_TableDelegate::ADN_TableDelegate( QTableView* parent /* = 0 */ )
    : gui::CommonDelegate( parent )
{
    gridPen_ = QPen( Qt::black, 2, Qt::SolidLine, Qt::RoundCap );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate destructor
// Created: ABR 2012-10-22
// -----------------------------------------------------------------------------
ADN_TableDelegate::~ADN_TableDelegate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColor
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColor( int fromRow, int toRow, int fromCol, int toCol, double min, double max )
{
    unsigned int id = GetNewId();
    colorPositions_.push_back( DelegatePosition( id, fromRow, toRow, fromCol, toCol ) );
    colors_[ id ] = std::pair< double, double >( min, max );
    return id;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColorOnColumn
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColorOnRow( int row, double min, double max )
{
    return AddColor( row, row, -1, -1, min, max );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColorOnColumn
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColorOnColumn( int col, double min, double max )
{
    return AddColor( -1, -1, col, col, min, max );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColorEditOnRow
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColorEditOnRow( int row )
{
    return AddColorEdit( row, row, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColorEditOnColumn
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColorEditOnColumn( int column )
{
    return AddColorEdit( -1, -1, column, column );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddColorEdit
// Created: LGY 2012-11-16
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddColorEdit( int fromRow, int toRow, int fromCol, int toCol )
{
    return AddSimpleWidget( fromRow, toRow, fromCol, toCol, colorEdits_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::GetColorType
// Created: ABR 2012-10-24
// -----------------------------------------------------------------------------
const std::pair< double, double >* ADN_TableDelegate::GetColorType( int row, int col ) const
{
    for( auto it = colorPositions_.begin(); it != colorPositions_.end(); ++it )
        if( ( it->fromRow_ == -1 || row >= it->fromRow_ ) &&
            ( it->toRow_ == -1   || row <= it->toRow_   ) &&
            ( it->fromCol_ == -1 || col >= it->fromCol_ ) &&
            ( it->toCol_ == -1   || col <= it->toCol_   ) )
            if( const std::pair< double, double >* color = Find( colors_, it->id_ ) )
                return color;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddDelayEditorOnRow
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddDelayEditOnRow( int row, unsigned int min )
{
    return AddDelayEdit( row, row, -1, -1, min );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddDelayEditorOnColumn
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddDelayEditOnColumn( int col, unsigned int min )
{
    return AddDelayEdit( -1, -1, col, col, min );
}


// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddTimeEditorOnRow
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddTimeEditOnRow( int row )
{
    return AddTimeEdit( row, row, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddTimeEditorOnColumn
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddTimeEditOnColumn( int col )
{
    return AddTimeEdit( -1, -1, col, col );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddTimeEditor
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddTimeEdit( int fromRow, int toRow, int fromCol, int toCol )
{
    return AddSimpleWidget( fromRow, toRow, fromCol, toCol, timeEdits_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddComboPtrInVectorOnRow
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddComboPtrInVectorOnRow( int row )
{
    return AddComboPtrInVector( row, row, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddComboPtrInVectorOnColumn
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddComboPtrInVectorOnColumn( int col )
{
    return AddComboPtrInVector( -1, -1, col, col );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddComboPtrInVector
// Created: ABR 2012-11-05
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddComboPtrInVector( int fromRow, int toRow, int fromCol, int toCol )
{
    return AddSimpleWidget( fromRow, toRow, fromCol, toCol, comboPtrInVectors_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddLocalizedLineEditOnRow
// Created: ABR 2013-08-20
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddLocalizedLineEditOnRow( int row )
{
    return AddLocalizedLineEdit( row, row, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddLocalizedLineEditOnColumn
// Created: ABR 2013-08-20
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddLocalizedLineEditOnColumn( int col )
{
    return AddLocalizedLineEdit( -1, -1, col, col );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddLocalizedLineEdit
// Created: ABR 2013-08-20
// -----------------------------------------------------------------------------
unsigned int ADN_TableDelegate::AddLocalizedLineEdit( int fromRow, int toRow, int fromCol, int toCol )
{
    return AddSimpleWidget( fromRow, toRow, fromCol, toCol, localizedEditLines_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddNewRowIndex
// Created: NPT 2012-11-08
// -----------------------------------------------------------------------------
void ADN_TableDelegate::AddBoldRowIndex( int row )
{
    boldGridRowIndexes_.insert( row );
}
// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::AddNewColumnIndex
// Created: NPT 2012-11-08
// -----------------------------------------------------------------------------
void ADN_TableDelegate::AddBoldColumnIndex( int column )
{
    boldGridColIndexes_.insert(  column );
}

// -----------------------------------------------------------------------------
// Name: std::set<int> ADN_TableDelegate::GetRowIndexes
// Created: NPT 2012-11-08
// -----------------------------------------------------------------------------
const std::set<int>& ADN_TableDelegate::GetBoldRowIndexes() const
{
    return boldGridRowIndexes_;
}

// -----------------------------------------------------------------------------
// Name: std::set<int> ADN_TableDelegate::GetColumnIndexes
// Created: NPT 2012-11-08
// -----------------------------------------------------------------------------
const std::set<int>& ADN_TableDelegate::GetBoldColumnIndexes() const
{
    return boldGridColIndexes_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::SetGridPen
// Created: NPT 2012-11-08
// -----------------------------------------------------------------------------
void ADN_TableDelegate::SetGridPen( QPen gridPen )
{
    gridPen_ = gridPen;
}

namespace
{
    class ADN_LocalizedEditor : public QWidget
    {
    public:
        explicit ADN_LocalizedEditor( QWidget* parent )
            : QWidget( parent )
        {
            editLine_ = new ADN_EditLine_LocalizedString( this );
            applyButton_ = new ADN_ApplyButton( this );
            applyButton_->Connect( editLine_ );
            QHBoxLayout* layout = new QHBoxLayout( this );
            layout->setMargin( 0 );
            layout->setSpacing( 0 );
            layout->addWidget( editLine_, 1 );
            layout->addWidget( applyButton_, 0 );
        }

        ADN_EditLine_LocalizedString* GetEditLine()
        {
            return editLine_;
        }

        virtual void focusInEvent( QFocusEvent* /*e*/ )
        {
            editLine_->setFocus( Qt::MouseFocusReason );
            editLine_->setSelection( 0, editLine_->text().size() );
        }

    private:
        ADN_EditLine_LocalizedString* editLine_;
        ADN_ApplyButton* applyButton_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::createEditor
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
QWidget* ADN_TableDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& index ) const
{
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    if( !position || readOnly_ )
        return 0;

    ADN_Connector_ABC* data = const_cast< ADN_Connector_ABC* >( static_cast< const ADN_Connector_ABC* >( index.model()->data( index, gui::Roles::SafeRole ).value< kernel::VariantPointer >().ptr_ ) );
    if( !data )
        return 0;

    // $$$$ ABR 2012-10-25: TODO, Create ADN_SpinBox_Int and Double which will inherit gui::RichSpinBox, so we won't need to re-define setModelData below
    if( const SpinBoxDescription< int >* element = Find( spinBoxs_, position->id_ ) )
    {
        std::pair< int, int > minMax = GetMinMax< int >( *element, newIndex, intRestrictions_, singleIntRestrictions_ );
        //ADN_SpinBox_Int* editor = new ADN_SpinBox_Int( parent, minMax.first, minMax.second, element->gap_ );
        ADN_EditLine_Int* editor = new ADN_EditLine_Int( parent );
        editor->GetValidator().setRange( minMax.first, minMax.second );
        editor->setAlignment( qApp->isRightToLeft() ? Qt::AlignLeft : Qt::AlignRight );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
    {
        std::pair< double, double > minMax = GetMinMax< double >( *element, newIndex, doubleRestrictions_, singleDoubleRestrictions_ );

        //ADN_SpinBox_Double* editor = new ADN_SpinBox_Double( parent, minMax.first, minMax.second, element->gap_, element->precision_ );
        ADN_EditLine_Double* editor = new ADN_EditLine_Double( parent );
        editor->GetValidator().setRange( minMax.first, minMax.second );
        editor->GetValidator().setDecimals( element->precision_ );
        editor->setAlignment( qApp->isRightToLeft() ? Qt::AlignLeft : Qt::AlignRight );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
    {
        ADN_ComboBox_Enum* editor = new ADN_ComboBox_Enum( parent );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( std::find( comboPtrInVectors_.begin(), comboPtrInVectors_.end(), position->id_ ) != comboPtrInVectors_.end() )
    {
        ADN_ComboBox_Vector* editor = new ADN_ComboBox_Vector( parent );
        editor->GetConnector().Connect( static_cast< ADN_Connector_Vector_ABC* >( data ) );
        return editor;
    }
    else if( const QString* element = Find( lineEdits_, position->id_ ) )
    {
        ADN_EditLine_String* editor = new ADN_EditLine_String( parent );
        if( !element->isEmpty() )
            editor->setValidator( new QRegExpValidator( QRegExp( *element ), editor ) );
        editor->GetConnector().Connect( data );
        editor->setAlignment( qApp->isRightToLeft() ? Qt::AlignRight : Qt::AlignLeft );
        return editor;
    }
    else if( std::find( checkboxes_.begin(), checkboxes_.end(), position->id_ ) != checkboxes_.end() )
    {
        return 0;
    }
    else if( const unsigned int* element = Find( delayEdits_, position->id_ ) )
    {
        ADN_TimeField* editor = new ADN_TimeField( parent );
        editor->SetMinimumValueInSecond( *element );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( std::find( timeEdits_.begin(), timeEdits_.end(), position->id_ ) != timeEdits_.end() )
    {
        ADN_TimeEdit* editor = new ADN_TimeEdit( parent );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( std::find( colorEdits_.begin(), colorEdits_.end(), position->id_ ) != colorEdits_.end() )
    {
        ADN_ColorSelector* editor = new ADN_ColorSelector( parent, false );
        editor->GetConnector().Connect( data );
        return editor;
    }
    else if( std::find( localizedEditLines_.begin(), localizedEditLines_.end(), position->id_ ) != localizedEditLines_.end() )
    {
        ADN_LocalizedEditor* editor = new ADN_LocalizedEditor( parent );
        ADN_Table* table = static_cast< ADN_Table* >( this->parent() );
        ADN_StandardItem* standardItem = static_cast< ADN_StandardItem* >( table->GetItemFromIndex( index ) );
        editor->GetEditLine()->ConnectWithRefValidity( *reinterpret_cast< const ADN_Ref_ABC* >( standardItem->GetData() ) );
        editor->GetEditLine()->GetConnector().Connect( data );
        editor->GetEditLine()->setAlignment( qApp->isRightToLeft() ? Qt::AlignRight : Qt::AlignLeft );
        return editor;
    }
    assert( false );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::setEditorData
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void ADN_TableDelegate::setEditorData( QWidget* /*editor*/, const QModelIndex& /*index*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::setModelData
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
void ADN_TableDelegate::setModelData( QWidget* editor, QAbstractItemModel* /*model*/, const QModelIndex& index ) const
{
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    ADN_Connector_ABC* data = const_cast< ADN_Connector_ABC* >( static_cast< const ADN_Connector_ABC* >( index.model()->data( index, gui::Roles::SafeRole ).value< kernel::VariantPointer >().ptr_ ) );
    if( !position || !data )
        return;
    ADN_Connector_ABC* guiConnector = 0;
    if( const SpinBoxDescription< int >* element = Find( spinBoxs_, position->id_ ) )
        guiConnector = &static_cast< ADN_EditLine_Int* >( editor )->GetConnector();
    else if( const SpinBoxDescription< double >* element = Find( doubleSpinBoxs_, position->id_ ) )
        guiConnector = &static_cast< ADN_EditLine_Double* >( editor )->GetConnector();
    else if( const QStringList* element = Find( comboBoxs_, position->id_ ) )
        guiConnector = &static_cast< ADN_ComboBox_Enum* >( editor )->GetConnector();
    else if( std::find( comboPtrInVectors_.begin(), comboPtrInVectors_.end(), position->id_ ) != comboPtrInVectors_.end() )
        guiConnector = &static_cast< ADN_ComboBox_Vector* >( editor )->GetConnector();
    else if( const QString* element = Find( lineEdits_, position->id_ ) )
        guiConnector = &static_cast< ADN_EditLine_String* >( editor )->GetConnector();
    else if( const unsigned int* element = Find( delayEdits_, position->id_ ) )
        guiConnector = &static_cast< ADN_TimeField* >( editor )->GetConnector();
    else if( std::find( timeEdits_.begin(), timeEdits_.end(), position->id_ ) != timeEdits_.end() )
        guiConnector = &static_cast< ADN_TimeEdit* >( editor )->GetConnector();
    else if( std::find( colorEdits_.begin(), colorEdits_.end(), position->id_ ) != colorEdits_.end() )
        guiConnector = &static_cast< ADN_ColorSelector* >( editor )->GetConnector();
    else if( std::find( localizedEditLines_.begin(), localizedEditLines_.end(), position->id_ ) != localizedEditLines_.end() )
        guiConnector = &static_cast< ADN_EditLine_LocalizedString* >( static_cast< ADN_LocalizedEditor* >( editor )->GetEditLine() )->GetConnector();
    if( guiConnector )
        guiConnector->Disconnect( data );
    static_cast< ADN_Table* >( this->parent() )->CheckValidity( newIndex.row(), newIndex.column() );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::editorEvent
// Created: NPT 2012-11-27
// -----------------------------------------------------------------------------
void ADN_TableDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Qt::ItemFlags flags = index.flags();
    if( ( flags & Qt::ItemIsUserCheckable ) )
    {
        // Draw background
        painter->save();
        drawBackground(painter, option, index);
        painter->restore();

        // Draw content
        QStyleOptionViewItemV4 viewItemOption( option );
        const int textMargin = QApplication::style()->pixelMetric( QStyle::PM_FocusFrameHMargin ) + 1;
        viewItemOption.decorationAlignment = Qt::AlignCenter;
        QRect newRect = QStyle::alignedRect( option.direction, Qt::AlignCenter,
                                             QSize( option.decorationSize.width() + 5, option.decorationSize.height() ),
                                             QRect( option.rect.x(), option.rect.y(), option.rect.width() - ( 2 * textMargin ), option.rect.height() ) );

        // Print original
        viewItemOption.rect = newRect;
        QItemDelegate::paint( painter, viewItemOption, index );
    }
    else
    {
         QStyleOptionViewItem tempOption = option;
         const CommonDelegate::DelegatePosition* position = IsInPosition( index.row(), index.column() );
         if( position )
             if( std::find( colorEdits_.begin(), colorEdits_.end(), position->id_ ) != colorEdits_.end() )
                 tempOption.state &= ~QStyle::State_Selected;
        QItemDelegate::paint( painter, tempOption, index );
    }

     // To draw a border on selected cells
     if( option.state & QStyle::State_Selected )
     {
         painter->save();
         QPen pen( Qt::black, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin );
         int w = pen.width() / 2;
         painter->setPen( pen );
         painter->drawRect( option.rect.adjusted( w, w, -w, -w ) );
         painter->restore();
     }
    QPen oldPen = painter->pen();
    painter->setPen( gridPen_ );
    if( boldGridRowIndexes_.find( index.row() ) != boldGridRowIndexes_.end() )
        painter->drawLine( option.rect.topRight(), option.rect.topLeft() );
    if( boldGridColIndexes_.find( index.column() )!= boldGridColIndexes_.end() )
        painter->drawLine( option.rect.topLeft(), option.rect.bottomLeft () );
    painter->setPen( oldPen );
}

// -----------------------------------------------------------------------------
// Name: ADN_TableDelegate::editorEvent
// Created: LGY 2012-11-27
// -----------------------------------------------------------------------------
bool ADN_TableDelegate::editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index )
{
    Qt::ItemFlags flags = model->flags( index );
    if( ( flags & Qt::ItemIsUserCheckable ) && ( flags & Qt::ItemIsEnabled ) )
    {
        QVariant value = index.data(Qt::CheckStateRole);
        if( !value.isValid() )
            return false;
        if( event->type() == QEvent::MouseButtonRelease )
        {
            const int textMargin = QApplication::style()->pixelMetric( QStyle::PM_FocusFrameHMargin ) + 1;
            QRect checkRect = QStyle::alignedRect( option.direction, Qt::AlignCenter,
                                                   option.decorationSize,
                                                   QRect( option.rect.x() + ( 2 * textMargin ), option.rect.y(),
                                                   option.rect.width() - ( 2 * textMargin ),
                                                   option.rect.height() ) );
            if( !checkRect.contains( static_cast< QMouseEvent* >( event )->pos() ) )
                return false;
        } else if( event->type() == QEvent::KeyPress )
        {
            if( static_cast< QKeyEvent* >( event )->key() != Qt::Key_Space &&
                static_cast<QKeyEvent*>( event )->key() != Qt::Key_Select )
                return false;
        } else
        {
            return false;
        }
        Qt::CheckState state =( static_cast< Qt::CheckState >( value.toInt() ) == Qt::Checked
                                                             ? Qt::Unchecked : Qt::Checked );
        return model->setData( index, state, Qt::CheckStateRole );
    }
    return QItemDelegate::editorEvent( event, model, option, index );
}
