// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MultiPercentage.h"
#include "moc_ADN_MultiPercentage.cpp"
#include "ADN_GuiBuilder.h"

#pragma warning( disable : 4129 )

// -----------------------------------------------------------------------------
// Name: ADN_MultiPercentage constructor
// Created: ABR 2011-03-18
// -----------------------------------------------------------------------------
ADN_MultiPercentage::ADN_MultiPercentage( Q3GroupBox* pParent, ADN_GuiBuilder& builder )
    : parent_ ( pParent )
    , builder_( builder )
    , warning_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiPercentage destructor
// Created: ABR 2011-03-18
// -----------------------------------------------------------------------------
ADN_MultiPercentage::~ADN_MultiPercentage()
{
    lines_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiPercentage::AddLine
// Created: ABR 2011-03-18
// -----------------------------------------------------------------------------
void ADN_MultiPercentage::AddLine( const char* szName, ADN_Connector_ABC*& itemConnector )
{
    ADN_EditLine_Double* pLine = builder_.AddField< ADN_EditLine_Double >( parent_, szName, itemConnector, tr( "%" ), ePercentage );
    connect( pLine, SIGNAL( textChanged( const QString& ) ), this, SLOT( PercentageChanged() ) );
    lines_.push_back( pLine );
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiPercentage::AddWarning
// Created: ABR 2011-03-18
// -----------------------------------------------------------------------------
void ADN_MultiPercentage::AddWarning()
{
    parent_->addSpace( 0 );
    warning_ = new QLabel( parent_ );
    parent_->addSpace( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiPercentage::PercentageChanged
// Created: ABR 2011-03-18
// -----------------------------------------------------------------------------
void ADN_MultiPercentage::PercentageChanged()
{
    double sum = 0;
    for( IT_EditLines it = lines_.begin(); it != lines_.end(); ++it )
    {
        double otherSum = 0;
        for( IT_EditLines it2 = lines_.begin(); it2 != lines_.end(); ++it2 )
            if( it != it2 )
                otherSum += ( *it2 )->text().toDouble();
//        ( *it )->GetValidator().setTop( 100 - otherSum );
        // $$$ LDC: Just does not work when the change is created by a signal:
        // It wrecks all the other values and switching from one element to the next results in nearly random values...
        sum += ( *it )->text().toDouble();
    }
    warning_->setText( ( sum != 100 ) ? "<font color=\"#FF0000\">" + tr( "Warning: only %1\% set, need 100\% or you won't be able to save." ).arg( sum ) + "</font>" : "" );
}
