//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Rich_ListViewItem.h $
// $Author: Ape $
// $Modtime: 28/02/05 11:14 $
// $Revision: 3 $
// $Workfile: ADN_Rich_ListViewItem.h $
//
//*****************************************************************************

#ifndef __ADN_Rich_ListViewItem_h_
#define __ADN_Rich_ListViewItem_h_

class ADN_ListView;
class ADN_ListViewItem;

//*****************************************************************************
// Created: SBO 2006-01-04
//*****************************************************************************
class ADN_Rich_ListViewItem
:   public Q3ListViewItem
{
public:
    //! @name Enums
    //@{
    enum E_FieldUnits
    {
        eUnitNone,
        eUnitHour
    };

    enum E_SortingConstraint
    {
        eSortingConstraint_Default,
        eSortingConstraint_First,
        eSortingConstraint_Last
    };
    //@}

public:
    explicit ADN_Rich_ListViewItem( ADN_ListView* parent, bool bGrid = false );
    explicit ADN_Rich_ListViewItem( Q3ListViewItem* parent, bool bGrid = false );
    virtual ~ADN_Rich_ListViewItem();

    //! @name Operations
    //@{
    void SetBackgroundColor( uint nColumn, const QColor& color );
    void SetWarning        ( uint nColumn );
    void SetError          ( uint nColumn );

    void OverrideSorting   ( E_SortingConstraint eConstraint );
    //@}

    template< typename T >
    void SetValueLowerThan( uint nColumn, const T& value, const T& error, const T& warning, E_FieldUnits eUnit = eUnitNone );
    template< typename T >
    void SetValueGreaterThan( uint nColumn, const T& value, const T& error, const T& warning, E_FieldUnits eUnit = eUnitNone );

    template< typename T >
    QString ToString( const T& value, E_FieldUnits eUnit = eUnitNone ) const;

protected:
    virtual void paintCell( QPainter* pPainter, const QColorGroup& cg, int nColumn, int nWidth, int nAlign );
    virtual QString key   ( int column, bool ascending ) const;

private:
    //! @name types
    //@{
    typedef std::vector< const QColor* > T_QColorVector;
    //@}

private:
    T_QColorVector      backgroundColors_;
    bool                bGrid_;
    E_SortingConstraint eSortingConstraint_;

    static QColor warningColor_;
    static QColor errorColor_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::SetValueLowerThan
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Rich_ListViewItem::SetValueLowerThan( uint nColumn, const T& value, const T& error, const T& warning, E_FieldUnits eUnit )
{
    if( value == std::numeric_limits< T >::min() )
        setText( nColumn, "-INF" );
    else
        setText( nColumn, ToString( value, eUnit ) );
    if( value > error )
        SetError( nColumn );
    else if( value > warning )
        SetWarning( nColumn );
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::SetValueGreaterThan
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Rich_ListViewItem::SetValueGreaterThan( uint nColumn, const T& value, const T& error, const T& warning, E_FieldUnits eUnit )
{
    if( value == std::numeric_limits< T >::max() )
        setText( nColumn, "+INF" );
    else
        setText( nColumn, ToString( value, eUnit ) );
    if( value < error )
        SetError( nColumn );
    else if( value < warning )
        SetWarning( nColumn );
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::ToString
// Created: SBO 2006-01-12
// -----------------------------------------------------------------------------
template< typename T >
QString ADN_Rich_ListViewItem::ToString( const T& value, E_FieldUnits eUnit /* = eUnitNone*/ ) const
{
    if( eUnit == eUnitHour )
    {
        T days    = ( uint )( value / 24 );
        T hours   = ( uint )( value - days * 24 );
        T minutes = ( uint )( value * 60 - days * 24 * 60 - hours * 60 );
        T seconds = ( uint )( value * 3600 - days * 24 * 3600 - hours * 3600 - minutes * 60 );
        QString result;
        if( days > 0 )
            result += QString::number( days ) + QObject::tr( "d", "ADN_Rich_ListViewItem" );
        if( hours > 0 )
            result += QString::number( hours ) + QObject::tr( "h", "ADN_Rich_ListViewItem" );
        if( minutes > 0 )
            result += QString::number( minutes ) + QObject::tr( "m", "ADN_Rich_ListViewItem" );
        if( seconds > 0 )
            result += QString::number( seconds ) + QObject::tr( "s", "ADN_Rich_ListViewItem" );
        return result;
    }
    return QString::number( value, 'f', 2 );
}

#endif // __ADN_Rich_ListViewItem_h_