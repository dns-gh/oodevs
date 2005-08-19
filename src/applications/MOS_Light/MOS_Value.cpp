//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Value.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 25 $
// $Workfile: MOS_Value.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#	pragma implementation
#endif

#include "MOS_Light_pch.h"

#include "MOS_Value.h"
#include "moc_MOS_Value.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_Value.inl"
#endif


#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_AgentManager.h"
#include "MOS_LifeBar.h"


#include <QLCDNumber.h>

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_Value constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_Value::MOS_Value( ASN1INT& value, QWidget* pParent, const std::string& sName, int nTools )
    : QFrame            ( pParent )
    , sName_            ( sName )
    , nTypeValue_       ( eTypeValueInt )
    , nTools_           ( nTools )
    , pCheckBox_        ( 0 )
    , pSlider_          ( 0 )
    , pPushButton_      ( 0 )
    , pLifeBar_         ( 0 )
    , pSpinBox_         ( 0 )
    , pSpeedo_          ( 0 )
    , pLabelVal_        ( 0 )
    , pTime_            ( 0 )

    , rValueMinWidget_  ( 0. )
    , rValueWidget_     ( 0. )
    , rValueMaxWidget_  ( 0. )
    , bModifiedWidget_  ( false )

    , rValueMinNetwork_  ( 0. )
    , rValueNetwork_     ( 0. )
    , rValueMaxNetwork_  ( 0. )
    , bModifiedNetwork_ ( false )

    , pAsnIntValue_     ( &value )
    , pAsnBoolValue_    ( 0 )
    , pAsnRealValue_    ( 0 )
    , nType_( -1 )
{
    Construct();
}


//-----------------------------------------------------------------------------
// Name: MOS_Value constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_Value::MOS_Value( ASN1BOOL& value, QWidget* pParent, const std::string& sName, int nTools )
    : QFrame            ( pParent )
    , sName_            ( sName )
    , nTypeValue_       ( eTypeValueBool )
    , nTools_           ( nTools )
    , pCheckBox_        ( 0 )
    , pSlider_          ( 0 )
    , pPushButton_      ( 0 )
    , pLifeBar_         ( 0 )
    , pSpinBox_         ( 0 )
    , pSpeedo_          ( 0 )
    , pLabelVal_        ( 0 )
    , pTime_            ( 0 )

    , rValueMinWidget_  ( 0. )
    , rValueWidget_     ( 0. )
    , rValueMaxWidget_  ( 0. )
    , bModifiedWidget_  ( false )

    , rValueMinNetwork_  ( 0. )
    , rValueNetwork_     ( 0. )
    , rValueMaxNetwork_  ( 0. )
    , bModifiedNetwork_ ( false )


    , pAsnIntValue_     ( 0 )
    , pAsnBoolValue_    ( &value )
    , pAsnRealValue_    ( 0 )
    , nType_( -1 )
{
    Construct();
}    

//-----------------------------------------------------------------------------
// Name: MOS_Value constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_Value::MOS_Value( ASN1REAL& value, QWidget* pParent, const std::string& sName, int nTools )
    : QFrame            ( pParent )
    , sName_            ( sName )
    , nTypeValue_       ( eTypeValueFloat )
    , nTools_           ( nTools )
    , pCheckBox_        ( 0 )
    , pSlider_          ( 0 )
    , pPushButton_      ( 0 )
    , pLifeBar_         ( 0 )
    , pSpinBox_         ( 0 )
    , pSpeedo_          ( 0 )
    , pLabelVal_        ( 0 )
    , pTime_            ( 0 )

    , rValueMinWidget_  ( 0. )
    , rValueWidget_     ( 0. )
    , rValueMaxWidget_  ( 0. )
    , bModifiedWidget_  ( false )

    , rValueMinNetwork_  ( 0. )
    , rValueNetwork_     ( 0. )
    , rValueMaxNetwork_  ( 0. )
    , bModifiedNetwork_ ( false )

    , pAsnIntValue_     ( 0 )
    , pAsnBoolValue_    ( 0 )
    , pAsnRealValue_    ( &value )
    , nType_( -1 )
{
    Construct();
}

//-----------------------------------------------------------------------------
// Name: MOS_Value constructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_Value::MOS_Value( E_TypeValue nTypeValue, QWidget* pParent, const std::string& sName, int nTools )
    : QFrame            ( pParent )
    , sName_            ( sName )
    , nTypeValue_       ( nTypeValue )
    , nTools_           ( nTools )
    , pCheckBox_        ( 0 )
    , pSlider_          ( 0 )
    , pPushButton_      ( 0 )
    , pLifeBar_         ( 0 )
    , pSpinBox_         ( 0 )
    , pSpeedo_          ( 0 )
    , pLabelVal_        ( 0 )
    , pTime_            ( 0 )

    , rValueMinWidget_  ( 0. )
    , rValueWidget_     ( 0. )
    , rValueMaxWidget_  ( 0. )
    , bModifiedWidget_  ( false )

    , rValueMinNetwork_  ( 0. )
    , rValueNetwork_     ( 0. )
    , rValueMaxNetwork_  ( 0. )
    , bModifiedNetwork_ ( false )

    , pAsnIntValue_     ( 0 )
    , pAsnBoolValue_    ( 0 )
    , pAsnRealValue_    ( 0 )
    , nType_( -1 )
{
    Construct();
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::Construct
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
void MOS_Value::Construct()
{
//    setMargin( 50 );
    setLineWidth( 1 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    pMainLayout_ = new QVBoxLayout( this );
    pMainLayout_->setMargin( 5 );
    pMainLayout_->setSpacing( 5 );

    int nMaxHeight = 20;
    
    if ( nTools_ & MOSToolValueCheck )
    {
        pCheckBox_ = new QCheckBox( sName_.c_str(), this );
        pMainLayout_->addWidget( pCheckBox_ );
        connect( pCheckBox_, SIGNAL( toggled( bool ) ), this, SLOT( SlotCheckBoxToggled( bool ) ) );
        if ( ( nTools_ & MOSToolValueModif ) == 0 )
        {
            pCheckBox_->setDisabled( true );
        }
        nMaxHeight += 20;
    }

    if ( nTools_ & MOSToolValueTitle )
    {
        pPushButton_ = new QPushButton( sName_.c_str(), this );
        pMainLayout_->addWidget( pPushButton_ );
        connect( pPushButton_, SIGNAL( pressed() ), this, SLOT( SlotButtonPressed() ) );
        nMaxHeight += 20;
    }

    if ( nTools_ & MOSToolValueText )
    {
        QHBoxLayout* pLabelLayout = new QHBoxLayout( pMainLayout_ );
        pLabelVal_ = new QLabel( this );
        pLabelVal_->setAlignment( AlignHCenter );
        pLabelLayout->addWidget( pLabelVal_ );
        nMaxHeight += 15;
    }

    if ( nTools_ & MOSToolValueSpeedo )
    {
        pSpeedo_ = new QDial( this );
        pMainLayout_->addWidget( pSpeedo_ );
        connect( pSpeedo_, SIGNAL( valueChanged(int) ), this, SLOT( SlotSetValueSpeedo( int ) ) );
        if ( ( nTools_ & MOSToolValueModif ) == 0 )
        {
            pSpeedo_->setDisabled( true );
        }
        nMaxHeight += 60;
    }

    if ( nTools_ & MOSToolValueSpin )
    {
        pSpinBox_ = new QSpinBox( this );
        pSpinBox_->setValue( rValueNetwork_ );
        pMainLayout_->addWidget( pSpinBox_ );
        connect( pSpinBox_, SIGNAL( valueChanged(int) ), this, SLOT( SlotSetValueSpinBox( int ) ) );
        if ( ( nTools_ & MOSToolValueModif ) == 0 )
        {
            pSpinBox_->setDisabled( true );
        }
        nMaxHeight += 20;
    }
    
    if ( nTools_ & MOSToolValueLifeBar )
    {
        pLifeBar_ = new MOS_LifeBar( this );
        pMainLayout_->addWidget( pLifeBar_ );
        nMaxHeight += 20;
    }

    if ( nTools_ & MOSToolValueTime )
    {
        pTime_ = new  QLCDNumber( 8, this );
        pTime_->setSegmentStyle( QLCDNumber::Filled );
        pTime_->setMinimumHeight( 40 );

        pMainLayout_->addWidget( pTime_ );
        nMaxHeight += 40;
    }

    if ( nTools_ & MOSToolValueSlide )
    {
        pSlider_ =  new QSlider( Horizontal, this );
        pSlider_->setTickmarks( QSlider::Below );
        pMainLayout_->addWidget( pSlider_ );
        connect( pSlider_, SIGNAL( valueChanged(int) ), this, SLOT( SlotSetValueSlider( int ) ) );
        if ( ( nTools_ & MOSToolValueModif ) == 0 )
        {
            pSlider_->setDisabled( true );
        }
        nMaxHeight += 40;
    }
    
    setMaximumHeight( nMaxHeight );
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::SlotButtonPressed
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_Value::SlotButtonPressed()
{
    Refresh();
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::Refresh
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_Value::Refresh()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::SlotSetValueSlider
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_Value::SlotSetValueSlider( int nValue )
{
    rValueWidget_ = (MT_Float)nValue;
    SetModifiedWidget( true );
    UpdateValue( rValueWidget_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::SlotSetValueSpinBox
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_Value::SlotSetValueSpinBox( int nValue )
{
    rValueWidget_ = (MT_Float)nValue;
    SetModifiedWidget( true );
    UpdateValue( rValueWidget_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::SlotCheckBoxToggled
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
void MOS_Value::SlotCheckBoxToggled( bool bState )
{
    rValueWidget_ = bState;
    SetModifiedWidget( true );
    UpdateValue( rValueWidget_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::SlotSetValueSpeedo
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_Value::SlotSetValueSpeedo( int nValue )
{
    rValueWidget_ = (MT_Float)nValue;
    SetModifiedWidget( true );
    UpdateValue( rValueWidget_ );
}
//-----------------------------------------------------------------------------
// Name: MOS_Value::UpdateValueMaxWidget
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
void MOS_Value::UpdateValueMax( MT_Float rValueMax )
{
    if ( pLifeBar_ )
        pLifeBar_->SetValueMax( rValueMax );

    if ( pSpinBox_ )
    {
        pSpinBox_->setMaxValue( rValueMax );
    }

    if ( pSlider_ )
    {
        pSlider_->setMaxValue( rValueMax );
        pSlider_->setPageStep( rValueMax / 10. );

    }

    if ( pSpeedo_ )
    {
        pSpeedo_->setMaxValue( rValueMax );
        pSpeedo_->setPageStep( rValueMax / 10. );
    }
   
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::UpdateValueMin
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
void MOS_Value::UpdateValueMin( MT_Float rValueMin )
{
    if ( pLifeBar_ )
        pLifeBar_->SetValueMin( rValueMin );

    if ( pSpinBox_ )
    {
        pSpinBox_->setMinValue( rValueMin );
    }

    if ( pSlider_ )
    {
        pSlider_->setMinValue( rValueMin );

    }

    if ( pSpeedo_ )
    {
        pSpeedo_->setMinValue( rValueMin );
    }
   
}
//-----------------------------------------------------------------------------
// Name: MOS_Value::UpdateValue
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_Value::UpdateValue( MT_Float rValue )
{
    if ( pCheckBox_ )
    {
        if( (int)rValue )
            pCheckBox_->setChecked( true );
        else
            pCheckBox_->setChecked( false );
    }

    if ( pLifeBar_ )
        pLifeBar_->SetValue( rValue );

    if ( pSpinBox_ )
    {
        if ( nTypeValue_ == eTypeValueFloat )
        {
            MT_Float rValue2 = rValue / 65536.;
            pSpinBox_->setValue( rValue2 );
        }
        else
        if ( nTypeValue_ == eTypeValueInt )
        {
            int nValue = rValue;
            pSpinBox_->setValue( nValue );
        }
        else
            pSpinBox_->setValue( rValue );
            
    }

    if ( pSlider_ )
        pSlider_->setValue( rValue );

    if ( pSpeedo_ )
        pSpeedo_->setValue( rValue );

    if ( pLabelVal_ )
    {
        if ( nTypeValue_ == eTypeValueFloat )
        {
            MT_Float rValue2 = rValue / 65536.;
            pLabelVal_->setText( MT_FormatString( "%f", rValue2 ).c_str() );
        }
        else
        if ( nTypeValue_ == eTypeValueInt )
        {
            int nValue = rValue;
            pLabelVal_->setText( MT_FormatString( "%d", nValue ).c_str() );
        }
    }

    if ( pTime_ )
    {
        if ( nTypeValue_ == eTypeValueFloat )
        {
            MT_Float rValue2 = rValue / 65536.;
            QString sTime( MT_FormatString( "%02d:%02d:%02d", ( (uint)rValue2 / 3600 ) % 24, ( (uint)rValue2 / 60 ) % 60 , (uint)rValue2 % 60  ).c_str() );
            pTime_->display( sTime );
        }
        else
        if ( nTypeValue_ == eTypeValueInt )
        {
            int nValue = rValue;
            QString sTime( MT_FormatString( "%02d:%02d:%02d", ( nValue / 3600 ) % 24, ( nValue / 60 ) % 60 , nValue % 60  ).c_str() );
            pTime_->display( sTime );
        }
    }

}


//-----------------------------------------------------------------------------
// Name: MOS_Value::CreateButtons
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_Value::CreateButtons()
{
}
//-----------------------------------------------------------------------------
// Name: MOS_Value destructor
// Created:  NLD 2002-01-03 
//-----------------------------------------------------------------------------
MOS_Value::~MOS_Value()
{
    
}


//=============================================================================
// SLOTS
//=============================================================================
//=============================================================================
// INITIALIZATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_Value::Initialize
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_Value::Initialize()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::Terminate
// Created: NLD 2002-08-23
//-----------------------------------------------------------------------------
void MOS_Value::Terminate()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::SetValueNetwork
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_Value::SetValueNetwork( MT_Float rValue )
{
    rValueNetwork_ = rValue;
    if ( nTypeValue_ == eTypeValueFloat )
        rValueNetwork_ *= 65536.;

    if ( rValueNetwork_ > rValueMaxNetwork_ )
        SetValueMaxNetwork( rValue );
    
    if ( rValueNetwork_ > rValueMaxWidget_ )
        SetValueMaxWidget( rValue );

    if ( !IsModifiedWidget() )
    {
        UpdateValue( rValueNetwork_ );
        SetModifiedWidget( false );
    }

    show();
}
//-----------------------------------------------------------------------------
// Name: MOS_Value::SetValue
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_Value::SetValueMaxNetwork( MT_Float rValueMax )
{
    rValueMaxNetwork_ = rValueMax;
    if ( nTypeValue_ == eTypeValueFloat )
        rValueMaxNetwork_ *= 65536.;
    
    UpdateValueMax( rValueMaxNetwork_ );
    show();
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::SetValue
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_Value::SetValueMinNetwork( MT_Float rValueMin )
{
    rValueMinNetwork_ = rValueMin;
    if ( nTypeValue_ == eTypeValueFloat )
        rValueMinNetwork_ *= 65536.;
    
    UpdateValueMin( rValueMinNetwork_ );
    show();
}


//-----------------------------------------------------------------------------
// Name: MOS_Value::SetValueWidget
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_Value::SetValueWidget( MT_Float rValue )
{
    rValueWidget_ = rValue;
    if ( nTypeValue_ == eTypeValueFloat )
        rValueWidget_ *= 65536.;

    if ( rValueWidget_ > rValueMaxWidget_ )
        SetValueMaxWidget( rValue );
    
    if ( rValueWidget_ > rValueMaxNetwork_ )
        SetValueMaxNetwork( rValue );

    UpdateValue( rValueWidget_ );

    show();
}
//-----------------------------------------------------------------------------
// Name: MOS_Value::SetValue
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_Value::SetValueMaxWidget( MT_Float rValueMax )
{
    rValueMaxWidget_ = rValueMax;
    if ( nTypeValue_ == eTypeValueFloat )
        rValueMaxWidget_ *= 65536.;
    
    UpdateValueMax( rValueMaxWidget_ );
    show();
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::SetValue
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_Value::SetValueMinWidget( MT_Float rValueMin )
{
    rValueMinWidget_ = rValueMin;
    if ( nTypeValue_ == eTypeValueFloat )
        rValueMinWidget_ *= 65536.;
    
    UpdateValueMin( rValueMinWidget_ );
    show();
}

//-----------------------------------------------------------------------------
// Name: MOS_Value::WriteMsg
// Created: FBD 03-01-21
//-----------------------------------------------------------------------------
void MOS_Value::WriteMsg( std::string& sParam )
{
   if ( nTypeValue_ == eTypeValueFloat )
   {
       assert( pAsnRealValue_ );
        *pAsnRealValue_ = (MT_Float)rValueWidget_ / 65536.;
        sParam += "------>";
        sParam += sName_;
        sParam += MT_FormatString( " :%f | ", rValueWidget_);

   }
   else if ( nTypeValue_ == eTypeValueInt )
   {
       assert( pAsnIntValue_ );
       *pAsnIntValue_ = rValueWidget_;
        sParam += "------>";
        sParam += sName_;
        sParam += MT_FormatString( " :%d | ", (uint)rValueWidget_ );
   }
   else if ( nTypeValue_ == eTypeValueBool )
   {
       assert( pAsnBoolValue_ );
       *pAsnBoolValue_ = rValueWidget_;
        sParam += "------>";
        sParam += sName_;
        sParam += MT_FormatString( " :%d | ", (uint)rValueWidget_ );
   }

}


//-----------------------------------------------------------------------------
// Name: MOS_Value::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_Value::FillRandomParameters()
{
   uint nMaxValue = (uint)(rValueMaxWidget_) + 1;
   
   if ( nTypeValue_ == eTypeValueFloat )
   {
        uint nValue = rand() % ( nMaxValue );
        rValueWidget_ = (MT_Float)( nValue );
        rValueWidget_ +=  (uint)rValueMinWidget_;
   }
   else if ( nTypeValue_ == eTypeValueInt )
   {
        uint nValue = rand() % ( nMaxValue );
        rValueWidget_ = (MT_Float)(nValue);
        rValueWidget_ +=  (uint)rValueMinWidget_;
   }
   else if ( nTypeValue_ == eTypeValueBool )
   {
        uint nValue = rand() % ( nMaxValue );
        rValueWidget_ = (MT_Float)(nValue);
        rValueWidget_ +=  (uint)rValueMinWidget_;
   }

}

//-----------------------------------------------------------------------------
// Name: MOS_Value::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_Value::ResaveParameters( MT_OutputArchive_ABC& archive )
{
//    uint nMaxValue = (uint)(rValueMaxWidget_);
    
   if ( nTypeValue_ == eTypeValueFloat )
   {
        archive .WriteField( "nValue", rValueWidget_ );
   }
   else if ( nTypeValue_ == eTypeValueInt )
   {
        archive .WriteField( "nValue", rValueWidget_ );
   }
   else if ( nTypeValue_ == eTypeValueBool )
   {
        archive .WriteField( "nValue", rValueWidget_ );
   }

}

//-----------------------------------------------------------------------------
// Name: MOS_Value::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_Value::ReloadParameters( MT_InputArchive_ABC& archive )
{
//    uint nMaxValue = (uint)(rValueMaxWidget_);
    
   if ( nTypeValue_ == eTypeValueFloat )
   {
        archive .ReadField( "nValue", rValueWidget_ );
   }
   else if ( nTypeValue_ == eTypeValueInt )
   {
        archive .ReadField( "nValue", rValueWidget_ );
   }
   else if ( nTypeValue_ == eTypeValueBool )
   {
        archive .ReadField( "nValue", rValueWidget_ );
   }

}

