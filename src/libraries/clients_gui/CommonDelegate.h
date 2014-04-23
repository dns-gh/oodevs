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
#include <tools/Resolver.h>

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

    //! @name QItemDelegate operations
    //@{
    virtual QWidget *createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    virtual void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
    virtual void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

    virtual void drawCheck( QPainter* painter, const QStyleOptionViewItem& option, const QRect& rect, Qt::CheckState state ) const;
    virtual bool editorEvent( QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index );
    //@}

    //! @name Operations
    //@{
    void Purge();
    void SetReadOnly( bool readOnly );
    bool IsReadOnly() const;
    bool IsCheckBox( const QModelIndex& index ) const;

    const QStringList* GetComboContent( int row, int col ) const;
    void SetComboContent( int row, int col, const QStringList& content );

    template< typename T >
    void SetSpinBoxMinMax( int row, int col, T min, T max );

    template< typename T >
    const T* Find( const std::map< unsigned int, T >& container, unsigned int key ) const;
    //@}

    //! @name Base operations
    //@{
    // FromRow/Col and ToRow/Col are used if value != -1
    // LinkedRow/Col is used if value != -1
    unsigned int AddSpinBox( int fromRow, int toRow, int fromCol, int toCol, int min = 0, int max = 100, int gap = 1 );
    unsigned int AddDoubleSpinBox( int fromRow, int toRow, int fromCol, int toCol, double min = 0., double max = 100., double gap = 1., int precision = 2 );
    unsigned int AddComboBox( int fromRow, int toRow, int fromCol, int toCol, QStringList stringList );
    template< typename T >
    unsigned int AddComboBox( int fromRow, int toRow, int fromCol, int toCol, T enumMax );
    unsigned int AddLineEdit( int fromRow, int toRow, int fromCol, int toCol, QString regExp = "" );
    unsigned int AddCheckBox( int fromRow, int toRow, int fromCol, int toCol );
    unsigned int AddDelayEdit( int fromRow, int toRow, int fromCol, int toCol, unsigned int min = 0u );
    //@}

    //! @name Row operations
    //@{
    unsigned int AddSpinBoxOnRow( int row, int min = 0, int max = 100, int gap = 1 );
    unsigned int AddDoubleSpinBoxOnRow( int row, double min = 0., double max = 100., double gap = 1., int precision = 2 );
    unsigned int AddComboBoxOnRow( int row, QStringList stringList );
    template< typename T >
    unsigned int AddComboBoxOnRow( int row, T enumMax );
    unsigned int AddLineEditOnRow( int row, QString regExp = "" );
    unsigned int AddCheckBoxOnRow( int row );
    //@}

    //! @name Column operations
    //@{
    unsigned int AddSpinBoxOnColumn( int column, int min = 0, int max = 100, int gap = 1 );
    unsigned int AddDoubleSpinBoxOnColumn( int column, double min = 0., double max = 100., double gap = 1., int precision = 2 );
    unsigned int AddComboBoxOnColumn( int column, QStringList stringList );
    template< typename T >
    unsigned int AddComboBoxOnColumn( int column, T enumMax );
    unsigned int AddLineEditOnColumn( int column, QString regExp = "" );
    unsigned int AddCheckBoxOnColumn( int column );
    //@}

    //! @name Public types
    //@{
    enum E_LinksType
    {
        eLT,
        eLTE,
        eGT,
        eGTE
        //eEQ,
        //eDIFF
    };
    //@}

    //! @name Links
    //@{
    template< typename T >
    void SetSingleColumnSumRestriction( int column, E_LinksType type, T sum );
    template< typename T >
    void SetColumnsSumRestriction( std::vector< int > columns, E_LinksType type, T sum );
    void SetColumnDependency( int dependentColumn, int referenceColumn, E_LinksType dependencyType );

    template< typename T >
    void SetSingleRowSumRestriction( int row, E_LinksType type, T sum );
    template< typename T >
    void SetRowsSumRestriction( std::vector< int > rows, E_LinksType type, T sum );
    void SetRowDependency( int dependentRow, int referenceRow, E_LinksType dependencyType );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnItemChanged( QStandardItem* item );
    //@}

signals:
    //! @name Signals
    //@{
    void CheckedStateChanged( const QStandardItem& );
    //@}

protected:
    //! @name Types
    //@{
    struct DelegatePosition
    {
        DelegatePosition() : id_( 0 ), fromRow_( 0 ), toRow_( 0 ), fromCol_( 0 ), toCol_( 0 ) {}
        DelegatePosition( unsigned int id, int fromRow, int toRow, int fromCol, int toCol ) : id_( id ), fromRow_( fromRow ), toRow_( toRow ), fromCol_( fromCol ), toCol_( toCol ) {}

        void Fill( unsigned int id, int fromRow, int toRow, int fromCol, int toCol )
        {
            id_ = id;
            fromRow_ = fromRow;
            toRow_ = toRow;
            fromCol_ = fromCol;
            toCol_ = toCol;
        }

        unsigned int id_;
        // $$$$ ABR 2012-10-24: Add a priority here to handle multi editor on the same item
        int fromRow_;
        int toRow_;
        int fromCol_;
        int toCol_;
    };
    typedef std::vector< DelegatePosition >    T_Positions;
    typedef T_Positions::const_iterator      CIT_Positions;

    template< typename T >
    struct SpinBoxDescription
    {
        SpinBoxDescription()
            : min_( 0 )
            , max_( 0 )
            , gap_( 0 )
            , precision_( 0 )
        {}
        SpinBoxDescription( T min, T max, T gap, int precision )
            : min_( min )
            , max_( max )
            , gap_( gap )
            , precision_( precision )
            {}

        T   min_;
        T   max_;
        T   gap_;
        int precision_;
    };

    struct Dependency
    {
        Dependency( int dependent, int reference, E_LinksType type, bool isRow = false )
            : dependent_( dependent )
            , reference_( reference )
            , type_( type )
            , isRow_( isRow )
            {}

        bool isRow_;
        int dependent_;
        int reference_;
        E_LinksType type_;
    };
    typedef std::vector< Dependency >         T_Dependencies;
    typedef T_Dependencies::const_iterator  CIT_Dependencies;

    template< typename T >
    struct SumRestriction
    {
        SumRestriction( std::vector< int > targets, E_LinksType type, T value, bool isRow = false )
            : targets_( targets )
            , type_( type )
            , value_( value )
            , isRow_( isRow )
        {}

        bool isRow_;
        std::vector< int > targets_;
        E_LinksType type_;
        T value_;
    };
    typedef std::vector< SumRestriction< int > >    T_IntSumRestrictions;
    typedef std::vector< SumRestriction< double > > T_DoubleSumRestrictions;

    typedef std::map< unsigned int, SpinBoxDescription< int > >    T_SpinBoxs;
    typedef std::map< unsigned int, SpinBoxDescription< double > > T_DoubleSpinBoxs;
    typedef std::map< unsigned int, QStringList >                  T_ComboBoxs;
    typedef std::map< unsigned int, QString >                      T_LineEdits;
    typedef std::map< unsigned int, unsigned int >                 T_DelayEdits;
    typedef std::vector< unsigned int >                            T_SimpleWidget;
    //@}

protected:
    //! @name Helpers
    //@{
    QModelIndex GetIndexFromSource( const QModelIndex& index ) const;

    template< typename Enum >
    void Populate( Enum size, QStringList& content ) const;

    template< typename T >
    std::pair< T, T > GetMinMax( const SpinBoxDescription< T >& spinbox, const QModelIndex& index, const std::vector< SumRestriction< T > >& sumRestriction, const std::vector< SumRestriction< T > >& singleSumRestriction ) const;

    unsigned int GetNewId();
    void CreatePosition( unsigned int id, int fromRow, int toRow, int fromCol, int toCol );
    const DelegatePosition* IsInPosition( int row, int col ) const;
    const DelegatePosition* FindPosition( int fromRow, int toRow, int fromCol, int toCol ) const;

    unsigned int AddSimpleWidget( int fromRow, int toRow, int fromCol, int toCol, T_SimpleWidget& vector );
    //@}

protected:
    //! @name Member data
    //@{
    T_SpinBoxs       spinBoxs_;
    T_DoubleSpinBoxs doubleSpinBoxs_;
    T_ComboBoxs      comboBoxs_;
    T_LineEdits      lineEdits_;
    T_SimpleWidget   checkboxes_;
    T_DelayEdits     delayEdits_;

    T_Positions                                     positions_;
    T_Dependencies                                  dependencies_;
    std::vector< SumRestriction< int > >            intRestrictions_;
    std::vector< SumRestriction< double > >         doubleRestrictions_;
    std::vector< SumRestriction< int > >            singleIntRestrictions_;
    std::vector< SumRestriction< double > >         singleDoubleRestrictions_;
    bool                                            readOnly_;

    unsigned int                                    currentId_;
    //@}
};

#include "CommonDelegate.inl"

}

#endif // __gui_CommonDelegate_h_
