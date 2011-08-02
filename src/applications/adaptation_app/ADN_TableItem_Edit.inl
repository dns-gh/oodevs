//*****************************************************************************
//
// $Created: JDY 03-07-09 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_TableItem_Edit.inl $
// $Author: Nld $
// $Modtime: 11/05/05 10:38 $
// $Revision: 8 $
// $Workfile: ADN_TableItem_Edit.inl $
//
//*****************************************************************************

#include <stdlib.h>
#include <QtGui/qlineedit.h>

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
template <class Connector,class Validator>
ADN_TableItem_Edit<Connector,Validator>::ADN_TableItem_Edit(ADN_Table*parent,void*data, Q3TableItem::EditType nEditType)
: ADN_TableItem_ABC(parent,data,nEditType)
, bUseColor_       ( false )
, rMax_            ( 100.f )
, rMin_            ( 0.f )
, nAlignment_      ( -1 )
{
    pConnector_ = new Connector(this);
    pValidator_ = new Validator(this);
    assert( pConnector_ != 0 );
    assert( pValidator_ != 0 );
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit destructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
template <class Connector,class Validator>
ADN_TableItem_Edit<Connector,Validator>::~ADN_TableItem_Edit()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit<Connector,Validator>::GetValidator
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
template <class Connector,class Validator>
Validator& ADN_TableItem_Edit<Connector,Validator>::GetValidator()
{
    return *pValidator_;
}


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit::SetValidator
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
template <class Connector,class Validator>
void ADN_TableItem_Edit<Connector,Validator>::SetValidator( Validator* pValidator )
{
    delete pValidator_;
    pValidator_ = pValidator;
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit<Connector,Validator>::createEditor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
template <class Connector,class Validator>
QWidget *ADN_TableItem_Edit<Connector,Validator>::createEditor() const
{
    QWidget* pEditor = Q3TableItem::createEditor();
    if ( pEditor && pEditor->inherits( "QLineEdit" ) )
    {
        QLineEdit* pLineEdit = static_cast<QLineEdit*>( pEditor );
        pLineEdit->setValidator( pValidator_ );
        pLineEdit->setAlignment( Qt::AlignRight );
        pLineEdit->setFrame( true );
        pLineEdit->selectAll();
    }
    return pEditor;
}


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit<Connector,Validator>::DoValueChanged
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
template <class Connector,class Validator>
void ADN_TableItem_Edit<Connector,Validator>::DoValueChanged()
{
    static_cast<Connector*>(pConnector_)->SetDataChanged(text());
}


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit::paint
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
template <class Connector,class Validator>
void ADN_TableItem_Edit<Connector,Validator>::paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected )
{
    if( ! bUseColor_ || selected )
    {
        ADN_TableItem_ABC::paint( p, cg, cr, selected );
        return;
    }

    bool bOk = true;
    double rValue = this->text().toDouble( &bOk );
    if( ! bOk )
    {
        ADN_TableItem_ABC::paint( p, cg, cr, selected );
        return;
    }

    double rRatio = (rValue - rMin_) / ( rMax_ - rMin_ );
    rRatio = std::max< double >( 0.0, rRatio );
    rRatio = std::min< double >( 1.0, rRatio );

    QColor backgroundColor;
    backgroundColor.setHsv( int( 120 * rRatio ), 30, 255 );

    QColorGroup newCg( cg );
    newCg.setColor( QColorGroup::Base, backgroundColor );

    ADN_TableItem_ABC::paint( p, newCg, cr, selected );
}


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit::UseColor
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
template <class Connector,class Validator>
void ADN_TableItem_Edit<Connector,Validator>::UseColor( bool bUseColor )
{
    bUseColor_ = bUseColor;
}


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit::SetRangeForColor
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
template <class Connector,class Validator>
void ADN_TableItem_Edit<Connector,Validator>::SetRangeForColor( double rMin, double rMax )
{
    assert( rMax > rMin );
    rMin_ = rMin;
    rMax_ = rMax;
}


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit::SetAlignment
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
template <class Connector,class Validator>
void ADN_TableItem_Edit<Connector,Validator>::SetAlignment( int nAlignment )
{
    nAlignment_ = nAlignment;
}


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_Edit::alignment
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
template <class Connector,class Validator>
int ADN_TableItem_Edit<Connector,Validator>::alignment() const
{
    if( nAlignment_ == -1 )
        return Q3TableItem::alignment();
    return nAlignment_;
}
