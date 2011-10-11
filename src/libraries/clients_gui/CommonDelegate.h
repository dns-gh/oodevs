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
#include "tools/Resolver.h"

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

    //! @name QItemDelegate Operations
    //@{
    QWidget *createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
    void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

    void Purge();
    //@}

    //! @name Operations
    //@{
    unsigned int AddSpinBox( int fromRow, int toRow, int fromCol, int toCol,
                             int min = 0, int max = 100, int gap = 1,
                             int minLinkedRow = -1, int maxLinkedRow = -1, int minLinkedCol = -1, int maxLinkedCol = -1 );
    unsigned int AddDoubleSpinBox( int fromRow, int toRow, int fromCol, int toCol,
                                   double min = 0., double max = 100., double gap = 1., int precision = 2,
                                   int minLinkedRow = -1, int maxLinkedRow = -1, int minLinkedCol = -1, int maxLinkedCol = -1 );

    unsigned int AddComboBox( int fromRow, int toRow, int fromCol, int toCol,
                              QStringList stringList );
    template< typename T >
    unsigned int AddComboBox( int fromRow, int toRow, int fromCol, int toCol,
                              T enumMax );
    //@}

    //! @name Row operations
    //@{
    unsigned int AddSpinBoxOnRow( int row, int min = 0, int max = 100, int gap = 1, int minLinkedRow = -1, int maxLinkedRow = -1 );
    unsigned int AddDoubleSpinBoxOnRow( int row, double min = 0., double max = 100., double gap = 1., int precision = 2, int minLinkedRow = -1, int maxLinkedRow = -1 );
    unsigned int AddComboBoxOnRow( int row, QStringList stringList );
    template< typename T >
    unsigned int AddComboBoxOnRow( int row, T enumMax );
    //@}

    //! @name Column operations
    //@{
    unsigned int AddSpinBoxOnColumn( int column, int min = 0, int max = 100, int gap = 1, int minLinkedCol = -1, int maxLinkedCol = -1 );
    unsigned int AddDoubleSpinBoxOnColumn( int column, double min = 0., double max = 100., double gap = 1., int precision = 2, int minLinkedCol = -1, int maxLinkedCol = -1 );
    unsigned int AddComboBoxOnColumn( int column, QStringList stringList );
    template< typename T >
    unsigned int AddComboBoxOnColumn( int column, T enumMax );
    //@}

private:
    //! @name Types
    //@{
    struct DelegatePosition
    {
        DelegatePosition() : id_( 0 ), fromRow_( 0 ), toRow_( 0 ), fromCol_( 0 ), toCol_( 0 ) {}
        DelegatePosition( unsigned int id, int fromRow, int toRow, int fromCol, int toCol ) : id_( id ), fromRow_( fromRow ), toRow_( toRow ), fromCol_( fromCol ), toCol_( toCol ) {}

        unsigned int id_;
        int fromRow_;
        int toRow_;
        int fromCol_;
        int toCol_;
    };

    template< typename T >
    struct SpinBoxDescription
    {
        SpinBoxDescription() : min_( 0 ), max_( 0 ), gap_( 0 ), precision_( 0 ), minLinkedColumn_( 0 ), maxLinkedColumn_( 0 ) {}
        SpinBoxDescription( T min, T max, T gap, int precision, int minLinkedRow, int maxLinkedRow, int minLinkedCol, int maxLinkedCol )
            : min_( min ), max_( max ), gap_( gap ), precision_( precision ), minLinkedRow_( minLinkedRow ), maxLinkedRow_( maxLinkedRow ), minLinkedCol_( minLinkedCol ), maxLinkedCol_( maxLinkedCol ) {}

        T   min_;
        T   max_;
        T   gap_;
        int precision_;
        int minLinkedRow_;
        int maxLinkedRow_;
        int minLinkedCol_;
        int maxLinkedCol_;
    };
    typedef std::vector< DelegatePosition >         T_Positions;
    typedef T_Positions::iterator                  IT_Positions;
    typedef T_Positions::const_iterator           CIT_Positions;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Enum >
    void Populate( Enum size, QStringList& content ) const;

    template< typename T >
    std::pair< T, T > GetMinMax( const SpinBoxDescription< T >& spinbox, const QModelIndex& index ) const;

    unsigned int GetNewId() const;
    const DelegatePosition* IsInPosition( int row, int col ) const;
    const DelegatePosition* FindPosition( int fromRow, int toRow, int fromCol, int toCol ) const;
    //@}

private:
    //! @name Member data
    //@{
    tools::Resolver< SpinBoxDescription< int > >    spinBoxs_;
    tools::Resolver< SpinBoxDescription< double > > doubleSpinBoxs_;
    tools::Resolver< QStringList >                  comboBoxs_;

    T_Positions                                     positions_;

    static unsigned int                             currentId_;
    //@}
};

#include "CommonDelegate.inl"

}

#endif // __gui_CommonDelegate_h_
