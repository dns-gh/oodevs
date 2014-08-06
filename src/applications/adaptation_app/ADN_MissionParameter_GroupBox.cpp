// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionParameter_GroupBox.h"
#include "moc_ADN_MissionParameter_GroupBox.cpp"
#include <boost/assign.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameter_GroupBox constructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionParameter_GroupBox::ADN_MissionParameter_GroupBox( int strips, Qt::Orientation orientation,
                                                              const QString& title, E_MissionParameterType authorized )
    : Q3GroupBox( strips, orientation, title )
    , authorized_( 1, authorized )
{
    hide();
}

ADN_MissionParameter_GroupBox::ADN_MissionParameter_GroupBox( int strips, Qt::Orientation orientation, const QString& title,
                                                              const std::vector< E_MissionParameterType >& authorized )
    : Q3GroupBox( strips, orientation, title )
    , authorized_( authorized )
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameter_GroupBox destructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionParameter_GroupBox::~ADN_MissionParameter_GroupBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionParameter_GroupBox::OnTypeChanged
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
void ADN_MissionParameter_GroupBox::OnTypeChanged( E_MissionParameterType type )
{
    setVisible( std::find( authorized_.begin(), authorized_.end(), type ) != authorized_.end() );
}
