//*****************************************************************************
//
// $Created: FBD 03-01-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ValueButtonList.inl $
// $Author: Fbd $
// $Modtime: 8/09/03 18:51 $
// $Revision: 14 $
// $Workfile: MOS_ValueButtonList.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList constructor
// Created: FBD 03-01-24
//-----------------------------------------------------------------------------

template <class T>
MOS_ValueButtonList<T>::MOS_ValueButtonList( T& output, QWidget* pParent, const std::string& sName, int nWidth, int nHeight, bool bExclusive )
    :   QButtonGroup        ( sName.c_str(), pParent )
    ,   MOS_MissionTool_ABC ()
    ,   output_             ( output )
    ,   nWidth_             ( nWidth )
    ,   nHeight_            ( nHeight )
    ,   nCurWidth_          ( 0 )
    ,   nCurHeight_         ( 0 )
    ,   bExclusive_         ( bExclusive )   
    ,   sName_              ( sName )
{
    pLayout_ = new QGridLayout( this, nWidth, nHeight );
    pLayout_->setMargin( 15 );
    setRadioButtonExclusive( bExclusive_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList destructor
// Created: FBD 03-01-24
//-----------------------------------------------------------------------------

template <class T>
MOS_ValueButtonList<T>::~MOS_ValueButtonList()
{
    while( valueItemVector_.size() )
    {

        DeleteValue( **valueItemVector_.begin() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList::CreateValue
// Created: FBD 03-01-24
//-----------------------------------------------------------------------------

template <class T>
MOS_ValueButton<T>& MOS_ValueButtonList<T>::CreateValue( const std::string& sName, T value, bool bSelected )
{
    MOS_ValueButton<T>& valueItem = *new MOS_ValueButton<T>( value, this, sName );
    valueItem.setChecked( bSelected );
    pLayout_->addWidget( &valueItem, nCurHeight_, nCurWidth_ );
    SetNextPosition();
    RegisterValue( valueItem );
    return valueItem;
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList::DeleteValue
// Created: FBD 03-01-24
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::DeleteValue( MOS_ValueButton<T>& valueItem )
{
    UnregisterValue( valueItem );
    delete &valueItem;
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList::RegisterValue
// Created: FBD 03-01-24
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::RegisterValue( MOS_ValueButton<T>& valueItem )
{
    valueItemVector_.push_back( &valueItem );
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList::UnregisterValue
// Created: FBD 03-01-24
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::UnregisterValue( MOS_ValueButton<T>& valueItem )
{
    IT_valueItemPtrVector itValueItem =  std::find( valueItemVector_.begin() , valueItemVector_.end(), &valueItem );
    assert( itValueItem != valueItemVector_.end() );
    valueItemVector_.erase( itValueItem );
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList::SetNextPosition
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::SetNextPosition()
{
    nCurWidth_++;
    if ( nCurWidth_ == nWidth_ )
    {
        nCurWidth_ = 0;
        ++nCurHeight_;
        assert ( nCurHeight_ != nCurWidth_ );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList constructor
// Created: NLD 03-03-01
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::WriteMsg( std::string& sParam )
{
    sParam += sName_;
    sParam += " : ";
    for ( IT_valueItemPtrVector itValue = valueItemVector_.begin() ; itValue != valueItemVector_.end() ; ++itValue )
    {
        if( (*itValue)->isChecked() )
        {
            (*itValue)->WriteMsg( output_, sParam );
            return;
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::FillRandomParameters( )
{
    uint nMaxSelected = valueItemVector_.size();
    uint nSelected = rand() % ( nMaxSelected );
    valueItemVector_[ nSelected ]->setChecked( true );
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    int nSelected = -1;
    QButton* pSelected = selected();
    if ( pSelected )
    {
        for ( nSelected = 0 ; nSelected < (int)valueItemVector_.size() ; ++nSelected )
        {
            if ( valueItemVector_[nSelected] == pSelected )
            {
                break;
            }
        }
    }
    archive.WriteField( "sName", std::string( "MOS_ValueButtonList|nSelected" ) );
    archive.WriteField( "nSelected", nSelected );
}



//-----------------------------------------------------------------------------
// Name: MOS_ValueButtonList::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::ReloadParameters( MT_InputArchive_ABC& archive )
{
//    uint nMaxSelected = valueItemVector_.size();
    uint nSelected = 0; //rand() % nMaxSelected;

// -------------------------------------------------
        std::string sName;
        archive.ReadField( "sName", sName );
        assert( sName == "MOS_ValueButtonList|nSelected" );
// -------------------------------------------------
    archive.ReadField( "nSelected", nSelected );
    if ( nSelected != -1 )
    {
        valueItemVector_[ nSelected ]->setChecked( true );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_ValueList::SetAgent
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::SetAgent( MOS_Agent* /*pAgent*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::SetWidgetTab
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::SetWidgetTab( QWidget* /*pWidget*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::SetMission
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::NotifyAgentHasMoved
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::Initialize
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::Terminate
// Created: FBD 03-02-05
//-----------------------------------------------------------------------------
template <class T>
void MOS_ValueButtonList<T>::Terminate()
{
}
