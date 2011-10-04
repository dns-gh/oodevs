// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_CommonDelegate_h_
#define __gui_CommonDelegate_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  CommonDelegate
    @brief  CommonDelegate
*/
// Created: ABR 2011-10-03
// =============================================================================
class CommonDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit CommonDelegate( QObject *parent = 0 );
    virtual ~CommonDelegate();
    //@}

    //! @name Operations
    //@{
    void AddSpinBox( int column, int min = 0, int max = 100, int gap = 1, int minLinkedColumn = -1, int maxLinkedColumn = -1 );
    void AddDoubleSpinBox( int column, double min = 0., double max = 100., double gap = 1., int precision = 2, int minLinkedColumn = -1, int maxLinkedColumn = -1 );
    void AddComboBox( int column, QStringList stringList );
    template< typename T >
    void AddComboBox( int column, T enumMax );
    //@}

    //! @name QItemDelegate Operations
    //@{
    QWidget *createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
    void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    //@}

private:

    //! @name Helpers
    //@{
    template< typename Enum >
    void Populate( Enum size, QStringList& content ) const;
    //@}

    //! @name Types
    //@{
    template< typename T >
    struct SpinBoxDescription
    {
        SpinBoxDescription() : min_( 0 ), max_( 0 ), gap_( 0 ), precision_( 0 ), minLinkedColumn_( 0 ), maxLinkedColumn_( 0 ) {}
        SpinBoxDescription( T min, T max, T gap, int precision, int minLinkedColumn, int maxLinkedColumn )
            : min_( min ), max_( max ), gap_( gap ), precision_( precision ), minLinkedColumn_( minLinkedColumn ), maxLinkedColumn_( maxLinkedColumn ) {}

        T   min_;
        T   max_;
        T   gap_;
        int precision_;
        int minLinkedColumn_;
        int maxLinkedColumn_;
    };

    typedef std::map< int, SpinBoxDescription< int > >    T_SpinBoxs;
    typedef T_SpinBoxs::const_iterator                  CIT_SpinBoxs;
    typedef std::map< int, SpinBoxDescription< double > > T_DoubleSpinBoxs;
    typedef T_DoubleSpinBoxs::const_iterator            CIT_DoubleSpinBoxs;
    typedef std::map< int, QStringList >                  T_ComboBoxs;
    typedef T_ComboBoxs::const_iterator                 CIT_ComboBoxs;
    //@}

private:
    //! @name Member data
    //@{
    T_SpinBoxs       spinBoxs_;
    T_DoubleSpinBoxs doubleSpinBoxs_;
    T_ComboBoxs      comboBoxs_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: CommonDelegate::AddComboBox
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
template< typename T >
void CommonDelegate::AddComboBox( int column, T enumMax )
{
    assert( spinBoxs_.find( column ) == spinBoxs_.end() && doubleSpinBoxs_.find( column ) == doubleSpinBoxs_.end() && comboBoxs_.find( column ) == comboBoxs_.end() );
    QStringList stringList;
    Populate( enumMax, stringList );
    comboBoxs_[ column ] = stringList;
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

}

#endif // __gui_CommonDelegate_h_
