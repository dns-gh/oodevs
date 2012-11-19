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

// -----------------------------------------------------------------------------
// Name: ADN_MultiPercentage::OnPercentageChanged
// Created: ABR 2012-11-19
// -----------------------------------------------------------------------------
template<>
void ADN_MultiPercentage< ADN_EditLine_Int >::OnPercentageChanged()
{
    int sum = 0;
    for( IT_EditLines it = lines_.begin(); it != lines_.end(); ++it )
    {
        int otherSum = 0;
        for( IT_EditLines it2 = lines_.begin(); it2 != lines_.end(); ++it2 )
            if( it != it2 )
                otherSum += ( *it2 )->text().toInt();
        ( *it )->GetValidator().setTop( 100 - otherSum );
        sum += ( *it )->text().toInt();
    }
    if( warning_ )
        warning_->setText( ( sum != 100 ) ? "<font color=\"#FF0000\">" + tools::translate( "ADN_MultiPercentage", "Warning: total of percentages must equal 100%." ) + "</font>" : "" );
}

// -----------------------------------------------------------------------------
// Name: ADN_MultiPercentage::OnPercentageChanged
// Created: ABR 2012-11-19
// -----------------------------------------------------------------------------
template<>
void ADN_MultiPercentage< ADN_EditLine_Double >::OnPercentageChanged()
{
    double sum = 0;
    for( IT_EditLines it = lines_.begin(); it != lines_.end(); ++it )
    {
        double otherSum = 0;
        for( IT_EditLines it2 = lines_.begin(); it2 != lines_.end(); ++it2 )
            if( it != it2 )
                otherSum += ( *it2 )->text().toDouble();
        ( *it )->GetValidator().setTop( 100 - otherSum );
        sum += ( *it )->text().toDouble();
    }
    if( warning_ )
        warning_->setText( ( sum != 100 ) ? "<font color=\"#FF0000\">" + tools::translate( "ADN_MultiPercentage", "Warning: total of percentages must equal 100%." ) + "</font>" : "" );
}