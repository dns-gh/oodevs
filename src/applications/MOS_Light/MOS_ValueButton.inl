//*****************************************************************************
//
// $Created: FBD 03-01-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ValueButton.inl $
// $Author: Fbd $
// $Modtime: 28/08/03 16:13 $
// $Revision: 6 $
// $Workfile: MOS_ValueButton.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_ValueButton constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
template <class T>
MOS_ValueButton<T>::MOS_ValueButton( T value, QWidget* pParent, const std::string& sName )
    : QRadioButton      ( sName.c_str(), pParent  )
    , value_            ( value )
    , sName_            ( sName )
{
    show();
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueButton destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
template <class T>
MOS_ValueButton<T>::~MOS_ValueButton()
{
}



//-----------------------------------------------------------------------------
// Name: MOS_ValueButton::WriteMsg
// Created: FBD 03-03-01
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButton<T>::WriteMsg( T& output, std::string& sParam )
{
    output = value_;
    sParam += "----->";
    sParam += sName_;
    sParam += MT_FormatString( ": %d | ", value_ );
}
