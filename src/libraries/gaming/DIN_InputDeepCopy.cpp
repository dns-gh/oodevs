// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "DIN_InputDeepCopy.h"
#include "DIN/DIN_Input.h"

using namespace DIN; using namespace NEK;

// -----------------------------------------------------------------------------
// Name: DIN_InputDeepCopy constructor
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
DIN_InputDeepCopy::DIN_InputDeepCopy( DIN_Input& input, T_Function function )
    : function_( function )
    , nekBuffer_( 0 )
    , nekInput_( 0 )
    , input_( 0 )
{
    const unsigned size = input.GetAvailable();
    const unsigned char* buffer = input.GetBuffer( size );
    
    nekBuffer_ = new NEK_Buffer( size );
    nekBuffer_->Append( buffer, size );
    nekInput_ = new NEK_Input( *nekBuffer_ );
    input_ = new DIN_Input( *nekInput_, size ); 
}

// -----------------------------------------------------------------------------
// Name: DIN_InputDeepCopy destructor
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
DIN_InputDeepCopy::~DIN_InputDeepCopy()
{
    delete input_;
    delete nekInput_;
    delete nekBuffer_;
}

// -----------------------------------------------------------------------------
// Name: DIN_InputDeepCopy::Apply
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void DIN_InputDeepCopy::Apply( AgentServerMsgMgr& manager )
{
    (manager.*function_)( *input_ );
}
