// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamGDH.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 5 $
// $Workfile: MOS_ParamGDH.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamGDH.h"
#include "moc_MOS_ParamGDH.cpp"

// -----------------------------------------------------------------------------
// Name: MOS_ParamGDH constructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
MOS_ParamGDH::MOS_ParamGDH( ASN1T_GDH& asnGDH, const std::string& strName, QWidget* pParent, bool bOptional )
    : QHBox         ( pParent )
    , MOS_Param_ABC ( bOptional )
    , strName_      ( strName )
    , asnGDH_       ( asnGDH )
{
    this->setSpacing( 5 );
    this->setSizePolicy( QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) );

    // Title
    MT_ParameterLabel* pLabel = new MT_ParameterLabel( strName_.c_str(), this, "" );
    pDateTimeEdit_ = new QDateTimeEdit( QDateTime::currentDateTime(), this );
    pCheckbox_ = new QCheckBox( this );

    pDateTimeEdit_->setEnabled( false );
    pCheckbox_->setChecked( false );

    this->setStretchFactor( pLabel, 2 );
    this->setStretchFactor( pDateTimeEdit_, 0 );
    this->setStretchFactor( pCheckbox_, 0 );

    connect( pCheckbox_, SIGNAL( toggled( bool ) ), SLOT( OnCheckboxToogled( bool ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamGDH destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
MOS_ParamGDH::~MOS_ParamGDH()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamGDH::FillRemotePopupMenu
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
void MOS_ParamGDH::FillRemotePopupMenu( QPopupMenu& /*popupMenu*/, const MOS_ActionContext& /*context*/ )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamGDH::CheckValidity
// Created: APE 2004-03-24
// -----------------------------------------------------------------------------
bool MOS_ParamGDH::CheckValidity()
{
    return true;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamGDH::WriteMsg
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
void MOS_ParamGDH::WriteMsg( std::stringstream& strMsg )
{
    strMsg << strName_ << ": ";

    if( pCheckbox_->isChecked() )
    {
        strMsg << pDateTimeEdit_->dateTime().toString();

        static QDateTime baseDateTime( QDate( 1901, 1, 1 ) );
        asnGDH_.qualificatif = EnumGDH_Qualificatif::at;
        asnGDH_.datation     = baseDateTime.secsTo( pDateTimeEdit_->dateTime() );
    }
    else
    {
        strMsg << "---";
        asnGDH_.qualificatif = EnumGDH_Qualificatif::at;
        asnGDH_.datation     = 0;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamGDH::OnCheckboxToogled
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
void MOS_ParamGDH::OnCheckboxToogled( bool b )
{
    pDateTimeEdit_->setEnabled( b );
}
