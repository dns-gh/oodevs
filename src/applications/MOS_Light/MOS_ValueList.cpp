//*****************************************************************************
//
// $Created: FBD 03-01-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ValueList.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 12 $
// $Workfile: MOS_ValueList.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_ValueList.h"

#ifndef MOS_USE_INLINE
#	include "MOS_ValueList.inl"
#endif
#include "MOS_Value.h"
#include "MOS_Agent.h"


#include <QLabel.h>

using namespace DIN;
//-----------------------------------------------------------------------------
// Name: MOS_ValueList constructor
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
MOS_ValueList::MOS_ValueList( QWidget* pParent, std::string sName, int nWidth, int nHeight )
    :   QFrame             ( pParent )
    ,   MOS_MissionTool_ABC()
    ,   pGridLayout_       ( 0 )
    ,   nWidth_            ( nWidth )
    ,   nHeight_           ( nHeight )
    ,   nCurWidth_         ( 0 )
    ,   nCurHeight_        ( 0 )
    ,   sName_             ( sName )
{
//    setMargin( 80 );
//    setLineWidth( 2 );
//    setFrameStyle( QFrame::Sunken | QFrame::Box );

    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );
    
    // Title
    QLabel* pLabel = CreateTitle( this, sName.c_str() );
    pLocalLayout->addWidget( pLabel );

    pGridLayout_ = new QGridLayout( pLocalLayout, nHeight_, nWidth_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList destructor
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
MOS_ValueList::~MOS_ValueList()
{
    // manque le delete ?
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::Initialize
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_ValueList::Initialize()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::Terminate
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_ValueList::Terminate()
{
}
//-----------------------------------------------------------------------------
// Name: MOS_ValueList::CreateValue
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
/*MOS_Value* MOS_ValueList::CreateValue( E_TypeValue nTypeValue, std::string sName, int nTool, MT_Float rValue )
{
    MOS_Value* pValue = new MOS_Value( nTypeValue, sName, this, nTool );
    pValue->SetValueWidget( rValue );
    RegisterValue( *pValue );
    pGridLayout_->addWidget( pValue, nCurHeight_, nCurWidth_ );
    
    SetNextPosition();
    return pValue;
}*/

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::DeleteValue
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_ValueList::DeleteValue( MOS_Value& value )
{
    UnregisterValue( value );
    delete &value;
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::SetNextPosition
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_ValueList::SetNextPosition()
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
// Name: MOS_ValueList::RegisterValue
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_ValueList::RegisterValue( MOS_Value& value )
{
    valueVector_.push_back( &value );
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::UnregisterValue
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_ValueList::UnregisterValue( MOS_Value& value )
{
    IT_ValuePtrVector itValue = std::find( valueVector_.begin(), valueVector_.end(), &value );
    assert ( itValue != valueVector_.end() );
    valueVector_.erase( itValue );
}


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::WriteMsg
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_ValueList::WriteMsg( std::string& sParam )
{
    sParam += sName_;
    
    for ( IT_ValuePtrVector itValue = valueVector_.begin() ; itValue != valueVector_.end() ; ++itValue )
    {
        (*itValue)->WriteMsg( sParam );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::WriteMsg
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: MOS_ValueList::FillRandomParameters
// Created: FBD 03-08-04
//-----------------------------------------------------------------------------
void MOS_ValueList::FillRandomParameters()
{
    for ( IT_ValuePtrVector itValue = valueVector_.begin() ; itValue != valueVector_.end() ; ++itValue )
    {
        (*itValue)->FillRandomParameters();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::ReloadParameters
// Created: FBD 03-08-04
//-----------------------------------------------------------------------------
void MOS_ValueList::ReloadParameters( MT_InputArchive_ABC& archive )
{
    for ( IT_ValuePtrVector itValue = valueVector_.begin() ; itValue != valueVector_.end() ; ++itValue )
    {
        (*itValue)->ReloadParameters( archive );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ValueList::ResaveParameters
// Created: FBD 03-08-04
//-----------------------------------------------------------------------------
void MOS_ValueList::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    for ( IT_ValuePtrVector itValue = valueVector_.begin() ; itValue != valueVector_.end() ; ++itValue )
    {
        (*itValue)->ResaveParameters( archive );
    }
}
