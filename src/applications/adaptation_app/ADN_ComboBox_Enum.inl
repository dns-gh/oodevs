//*****************************************************************************
//
// $Created: JDY 03-08-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ComboBox_Enum.inl $
// $Author: Ape $
// $Modtime: 10/02/05 15:46 $
// $Revision: 2 $
// $Workfile: ADN_ComboBox_Enum.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_Enum constructor
// Created: JDY 03-08-29
//-----------------------------------------------------------------------------
template <class EnumType>
ADN_ComboBox_Enum<EnumType>::ADN_ComboBox_Enum(T_Converter cv,QWidget * parent, const char * name) 
: ADN_ComboBox(parent,name)
, converter_(cv)
{   
    pConnector_=new ADN_CCB(*this); 
    assert(pConnector_); 
}


//-----------------------------------------------------------------------------
// Name: ADN_ComboBox_Enum destructor
// Created: JDY 03-08-29
//-----------------------------------------------------------------------------
template <class EnumType>
ADN_ComboBox_Enum<EnumType>::~ADN_ComboBox_Enum()
{   
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ComboBox_Enum::UpdateEnableState
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
template <class EnumType>
void ADN_ComboBox_Enum< EnumType >::UpdateEnableState()
{
    if( bEnabledOnlyInAdminMode_ && IsAutoEnabled() )
        setEnabled( static_cast< ADN_Connector_EnumComboBox* >( pConnector_ )->IsConnected() );
}
