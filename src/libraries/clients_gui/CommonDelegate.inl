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
unsigned int CommonDelegate::AddSpinBoxOnRow( int row, int min /*= 0*/, int max /*= 100*/, int gap /*= 1*/ )
{
    return AddSpinBox( row, row, -1, -1, min, max, gap );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddDoubleSpinBoxOnRow
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddDoubleSpinBoxOnRow( int row, double min /*= 0.*/, double max /*= 100.*/, double gap /*= 1.*/, int precision /*= 2*/ )
{
    return AddDoubleSpinBox( row, row, -1, -1, min, max, gap, precision );
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

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddLineEditOnRow
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddLineEditOnRow( int row, QString regExp /*= ""*/ )
{
    return AddLineEdit( row, row, -1, -1, regExp );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddCheckBoxOnRow
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddCheckBoxOnRow( int row )
{
    return AddCheckBox( row, row, -1, -1 );
}

// =============================================================================
// OnColumn
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddSpinBoxOnColumn
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddSpinBoxOnColumn( int col, int min /*= 0*/, int max /*= 100*/, int gap /*= 1*/ )
{
    return AddSpinBox( -1, -1, col, col, min, max, gap );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddDoubleSpinBoxOnColumn
// Created: ABR 2011-10-10
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddDoubleSpinBoxOnColumn( int col, double min /*= 0.*/, double max /*= 100.*/, double gap /*= 1.*/, int precision /*= 2*/ )
{
    return AddDoubleSpinBox( -1, -1, col, col, min, max, gap, precision );
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

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddLineEditOnColumn
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddLineEditOnColumn( int col, QString regExp /*= ""*/ )
{
    return AddLineEdit( -1, -1, col, col, regExp );
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddCheckBoxOnColumn
// Created: ABR 2012-10-23
// -----------------------------------------------------------------------------
inline
unsigned int CommonDelegate::AddCheckBoxOnColumn( int col )
{
    return AddCheckBox( -1, -1, col, col );
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

namespace
{
    void UpdateMinMax( int& minimum, int& maximum, CommonDelegate::E_LinksType linkType, int value )
    {
        switch( linkType )
        {
        case CommonDelegate::eLT:
            maximum = std::min< int >( maximum, value - 1 );
            break;
        case CommonDelegate::eLTE:
            maximum = std::min< int >( maximum, value );
            break;
        case CommonDelegate::eGT:
            minimum = std::max< int >( minimum, value + 1 );
            break;
        case CommonDelegate::eGTE:
            minimum = std::max< int >( minimum, value );
            break;
        default:
            break;
        }
    }

    void UpdateMinMax( double& minimum, double& maximum, CommonDelegate::E_LinksType linkType, double value )
    {
        switch( linkType )
        {
        case CommonDelegate::eLT:
            maximum = std::min< double >( maximum, value - 0.01 );
            break;
        case CommonDelegate::eLTE:
            maximum = std::min< double >( maximum, value );
            break;
        case CommonDelegate::eGT:
            minimum = std::max< double >( minimum, value + 0.01 );
            break;
        case CommonDelegate::eGTE:
            minimum = std::max< double >( minimum, value );
            break;
        default:
            break;
        }
    }

    template< typename T >
    T GetValueFromString( const QLocale&, const QString& )
    {
        return T();
    }

    template<>
    int GetValueFromString< int >( const QLocale& locale, const QString& text )
    {
        return locale.toInt( text );
    }

    template<>
    double GetValueFromString< double >( const QLocale& locale, const QString& text )
    {
        return locale.toDouble( text );
    }

}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::GetMinMax
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
template< typename T >
inline
std::pair< T, T > CommonDelegate::GetMinMax( const SpinBoxDescription< T >& spinbox, const QModelIndex& index, const std::vector< SumRestriction< T > >& sumRestriction, const std::vector< SumRestriction< T > >& singleSumRestriction ) const
{
    static QLocale locale;
    T minimum = spinbox.min_;
    T maximum = spinbox.max_;
    QModelIndex newIndex = GetIndexFromSource( index );
    const QStandardItemModel* model = static_cast< const QStandardItemModel* >( newIndex.model() );

    if( model )
    {
        // Look for dependencies
        for( auto it = dependencies_.begin(); it != dependencies_.end(); ++it )
        {
            int targetRow = newIndex.row();
            int targetColumn = newIndex.column();
            if( it->isRow_ && it->dependent_ == newIndex.row() )
                targetRow = it->reference_;
            else if( !it->isRow_ && it->dependent_ == newIndex.column() )
                targetColumn = it->reference_;

            if( targetRow != newIndex.row() || targetColumn != newIndex.column() )
                UpdateMinMax( minimum, maximum, it->type_, GetValueFromString< T >( locale, model->item( targetRow, targetColumn )->data( Qt::EditRole ).toString() ) );
        }

        // Look for single restrictions
        for( std::vector< SumRestriction< T > >::const_iterator it = singleSumRestriction.begin(); it != singleSumRestriction.end(); ++it )
        {
            int targetRow = newIndex.row();
            int targetColumn = newIndex.column();
            T sum = 0;
            assert( it->targets_.size() == 1 );
            if( it->isRow_ && newIndex.row() == it->targets_[ 0 ] )
            {
                for( int col = 0; col < model->columnCount(); ++col )
                    if( col != targetColumn )
                        sum += GetValueFromString< T >( locale, model->item( targetRow, col )->data( Qt::EditRole ).toString() );
                UpdateMinMax( minimum, maximum, it->type_, it->value_ - sum );
            }
            else if( !it->isRow_ && newIndex.column() == it->targets_[ 0 ] )
            {
                for( int row = 0; row < model->rowCount(); ++row )
                    if( row != targetRow )
                        sum += GetValueFromString< T >( locale, model->item( row, targetColumn )->data( Qt::EditRole ).toString() );
                UpdateMinMax( minimum, maximum, it->type_, it->value_ - sum );
            }
        }

        // Look for restrictions
        for( std::vector< SumRestriction< T > >::const_iterator it = sumRestriction.begin(); it != sumRestriction.end(); ++it )
        {
            int targetRow = newIndex.row();
            int targetColumn = newIndex.column();
            T sum = 0;
            if( it->isRow_ && std::find( it->targets_.begin(), it->targets_.end(), newIndex.row() ) != it->targets_.end() )
            {
                for( std::vector< int >::const_iterator row = it->targets_.begin(); row != it->targets_.end(); ++row )
                    if( *row != newIndex.row() )
                        sum += GetValueFromString< T >( locale, model->item( *row, targetColumn )->data( Qt::EditRole ).toString() );
                UpdateMinMax( minimum, maximum, it->type_, it->value_ - sum );
            }
            else if( !it->isRow_ && std::find( it->targets_.begin(), it->targets_.end(), newIndex.column() ) != it->targets_.end() )
            {
                for( std::vector< int >::const_iterator col = it->targets_.begin(); col != it->targets_.end(); ++col )
                    if( *col != newIndex.column() )
                        sum += GetValueFromString< T >( locale, model->item( targetRow, *col )->data( Qt::EditRole ).toString() );
                UpdateMinMax( minimum, maximum, it->type_, it->value_ - sum );
            }
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
    spinBoxs_.clear();
    doubleSpinBoxs_.clear();
    comboBoxs_.clear();
    lineEdits_.clear();
    checkboxes_.clear();
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::FindPosition
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
const CommonDelegate::DelegatePosition* CommonDelegate::FindPosition( int fromRow, int toRow, int fromCol, int toCol ) const
{
    for( auto it = positions_.begin(); it != positions_.end(); ++it )
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
    for( auto it = positions_.begin(); it != positions_.end(); ++it )
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
// Name: CommonDelegate::IsCheckBox
// Created: ABR 2012-10-30
// -----------------------------------------------------------------------------
inline
bool CommonDelegate::IsCheckBox( const QModelIndex& index ) const
{
    QModelIndex newIndex = GetIndexFromSource( index );
    const CommonDelegate::DelegatePosition* position = IsInPosition( newIndex.row(), newIndex.column() );
    if( !position )
        return false;
    if( std::find( checkboxes_.begin(), checkboxes_.end(), position->id_ ) != checkboxes_.end() )
        return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::GetComboContent
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
const QStringList* CommonDelegate::GetComboContent( int row, int col ) const
{
    const CommonDelegate::DelegatePosition* position = IsInPosition( row, col );
    const QStringList* element = 0;
    if( position )
    {
        T_ComboBoxs::const_iterator it = comboBoxs_.find( position->id_ );
        if( it != comboBoxs_.end() )
            element = &it->second;
    }
    return element;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::SetComboContent
// Created: NPT 2012-10-19
// -----------------------------------------------------------------------------
inline
void CommonDelegate::SetComboContent( int row, int col, const QStringList& content )
{
    const CommonDelegate::DelegatePosition* position = IsInPosition( row, col );
    if( position )
        comboBoxs_[ position->id_ ] = content;
}

// -----------------------------------------------------------------------------
// Name: CommonDelegate::Find
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
template< typename T >
inline
const T* CommonDelegate::Find( const std::map< unsigned int, T >& container, unsigned int key ) const
{
    std::map< unsigned int, T >::const_iterator it = container.find( key );
    if( it != container.end() )
        return &it->second;
    return 0;
}
