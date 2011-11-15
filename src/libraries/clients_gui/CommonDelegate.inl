// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

// =============================================================================
// OnRow
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddSpinBoxOnRow
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddSpinBoxOnRow( int row, int min /*= 0*/, int max /*= 100*/, int gap /*= 1*/,
                                              int minLinkedRow /*= -1*/, int maxLinkedRow /*= -1*/ )
{
    return AddSpinBox( row, row, -1, -1, min, max, gap, minLinkedRow, maxLinkedRow, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddDoubleSpinBoxOnRow
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddDoubleSpinBoxOnRow( int row, double min /*= 0.*/, double max /*= 100.*/, double gap /*= 1.*/, int precision /*= 2*/,
                                                    int minLinkedRow /*= -1*/, int maxLinkedRow /*= -1*/ )
{
    return AddDoubleSpinBox( row, row, -1, -1, min, max, gap, precision, minLinkedRow, maxLinkedRow, -1, -1 );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBoxOnRow
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddComboBoxOnRow( int row, QStringList stringList )
{
    return AddComboBox( row, row, -1, -1, stringList );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBoxOnRow
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
template< typename T >
inline
unsigned int CommonDelegate::AddComboBoxOnRow( int row, T enumMax )
{
    return AddComboBox( row, row, -1, -1, enumMax );
}

// =============================================================================
// OnColumn
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddSpinBoxOnColumn
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddSpinBoxOnColumn( int col, int min /*= 0*/, int max /*= 100*/, int gap /*= 1*/,
                                                 int minLinkedCol /*= -1*/, int maxLinkedCol/*= -1*/ )
{
    return AddSpinBox( -1, -1, col, col, min, max, gap, -1, -1, minLinkedCol, maxLinkedCol );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddDoubleSpinBoxOnColumn
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddDoubleSpinBoxOnColumn( int col, double min /*= 0.*/, double max /*= 100.*/, double gap /*= 1.*/, int precision /*= 2*/,
                                                       int minLinkedCol /*= -1*/, int maxLinkedCol /*= -1*/ )
{
    return AddDoubleSpinBox( -1, -1, col, col, min, max, gap, precision, -1, -1, minLinkedCol, maxLinkedCol );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBoxOnColumn
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddComboBoxOnColumn( int col, QStringList stringList )
{
    return AddComboBox( -1, -1, col, col, stringList );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBoxOnColumn
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
template< typename T >
inline
unsigned int CommonDelegate::AddComboBoxOnColumn( int col, T enumMax )
{
    return AddComboBox( -1, -1, col, col, enumMax );
}

// =============================================================================
// Methods
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBox
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
template< typename T >
inline
unsigned int CommonDelegate::AddComboBox( int fromRow, int toRow, int fromCol, int toCol, T enumMax )
{
    QStringList stringList;
    Populate( enumMax, stringList );
    return AddComboBox( fromRow, toRow, fromCol, toCol, stringList );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::Populate
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
template< typename Enum >
void CommonDelegate::Populate( Enum size, QStringList& content ) const
{
    for( unsigned int i = 0; i < unsigned int( size ); ++i )
        content << tools::ToString( static_cast< Enum >( i ) );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::GetNewId
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::GetNewId()
{
    return currentId_++;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::GetIndexFromSource
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
QModelIndex CommonDelegate::GetIndexFromSource( const QModelIndex& index ) const
{
    QModelIndex newIndex = index;
    if( dynamic_cast< const QSortFilterProxyModel* >( index.model() ) )
    {
        const QSortFilterProxyModel* proxyModel = static_cast< const QSortFilterProxyModel* >( index.model() );
        newIndex = proxyModel->mapToSource( index );
    }
    return newIndex.isValid() ? newIndex : index;
}

// -----------------------------------------------------------------------------
// Name: std::pair< T, T > CommonDelegate::GetMinMax
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
template< typename T >
inline
std::pair< T, T > CommonDelegate::GetMinMax( const SpinBoxDescription< T >& spinbox, const QModelIndex& index ) const
{
    T minimum = spinbox.min_;
    T maximum = spinbox.max_;
    QModelIndex newIndex = GetIndexFromSource( index );
    const QStandardItemModel* model = static_cast< const QStandardItemModel* >( newIndex.model() );// GetDataModelFromSource( index );
    if( model )
    {
        if( spinbox.minLinkedRow_ != -1 || spinbox.minLinkedCol_ != -1 )
        {
            int minRow = ( spinbox.minLinkedRow_ == -1 ) ? newIndex.row() : spinbox.minLinkedRow_;
            int minCol = ( spinbox.minLinkedCol_ == -1 ) ? newIndex.column() : spinbox.minLinkedCol_;
            minimum = static_cast< T >( model->item( minRow, minCol )->data( Qt::EditRole ).toDouble() );
        }
        if( spinbox.maxLinkedRow_ != -1 || spinbox.maxLinkedCol_ != -1 )
        {
            int maxRow = ( spinbox.maxLinkedRow_ == -1 ) ? newIndex.row() : spinbox.maxLinkedRow_;
            int maxCol = ( spinbox.maxLinkedCol_ == -1 ) ? newIndex.column() : spinbox.maxLinkedCol_;
            maximum = static_cast< T >( model->item( maxRow, maxCol )->data( Qt::EditRole ).toDouble() );
        }
    }
    return std::pair< T, T >( minimum, maximum );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::Purge
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
void CommonDelegate::Purge()
{
    positions_.clear();
    spinBoxs_.DeleteAll();
    doubleSpinBoxs_.DeleteAll();
    comboBoxs_.DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::FindPosition
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
const CommonDelegate::DelegatePosition* CommonDelegate::FindPosition( int fromRow, int toRow, int fromCol, int toCol ) const
{
    for( CIT_Positions it = positions_.begin(); it != positions_.end(); ++it )
        if( ( it->fromRow_ == -1 && fromRow == -1 || fromRow >= it->fromRow_ ) &&
            ( it->toRow_ == -1   && toRow   == -1 || toRow <= it->toRow_     ) &&
            ( it->fromCol_ == -1 && fromCol == -1 || fromCol >= it->fromCol_ ) &&
            ( it->toCol_ == -1   && toCol   == -1 || toCol <= it->toCol_   ) )
            return &*it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::IsInPosition
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
const CommonDelegate::DelegatePosition* CommonDelegate::IsInPosition( int row, int col ) const
{
    for( CIT_Positions it = positions_.begin(); it != positions_.end(); ++it )
        if( ( it->fromRow_ == -1 || row >= it->fromRow_ ) &&
            ( it->toRow_ == -1   || row <= it->toRow_   ) &&
            ( it->fromCol_ == -1 || col >= it->fromCol_ ) &&
            ( it->toCol_ == -1   || col <= it->toCol_   ) )
            return &*it;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::updateEditorGeometry
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
inline
void CommonDelegate::updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& /*index */ ) const
{
    editor->setGeometry(option.rect);
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::SetReadOnly
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
void CommonDelegate::SetReadOnly( bool readOnly )
{
    readOnly_ = readOnly;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::IsReadOnly
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
bool CommonDelegate::IsReadOnly() const
{
    return readOnly_;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::GetComboContent
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
const QStringList* CommonDelegate::GetComboContent( int row, int col ) const
{
    const CommonDelegate::DelegatePosition* position = IsInPosition( row, col );
    QStringList* element = 0;
    if( position )
         element = comboBoxs_.Find( position->id_ );
    return element;
}
