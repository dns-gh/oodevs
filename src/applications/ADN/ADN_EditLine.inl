//*****************************************************************************
//
// $Created: JDY 03-07-01 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_EditLine.inl $
// $Author: Ape $
// $Modtime: 14/04/05 16:45 $
// $Revision: 5 $
// $Workfile: ADN_EditLine.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: ADN_EditLine constructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
template <class Connector, class Validator>
ADN_EditLine<Connector, Validator>::ADN_EditLine( QWidget* parent, const char * name)
: ADN_EditLine_ABC(parent,name)
{
    pConnector_ = new Connector( this );
    pValidator_ = new Validator( this );
    this->setValidator( pValidator_ );
    assert( pConnector_ != 0 );
}


//-----------------------------------------------------------------------------
// Name: ADN_EditLine destructor
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
template <class Connector, class Validator>
ADN_EditLine<Connector, Validator>::~ADN_EditLine()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_EditLine::GetValidator
// Created: APE 2005-04-14
// -----------------------------------------------------------------------------
template <class Connector, class Validator>
Validator& ADN_EditLine<Connector, Validator>::GetValidator()
{
    return *pValidator_;
}


//-----------------------------------------------------------------------------
// Name: ADN_EditLine<Connector,Validator>::TextChanged
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
template <class Connector, class Validator>
void ADN_EditLine<Connector, Validator>::TextChanged( const QString& string )
{
    static_cast<Connector*>(pConnector_)->SetDataChanged(string);
}


// -----------------------------------------------------------------------------
// Name: ADN_EditLine::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template <class Connector, class Validator>
void ADN_EditLine<Connector, Validator>::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< Connector* >( pConnector_ )->IsConnected() );
}
