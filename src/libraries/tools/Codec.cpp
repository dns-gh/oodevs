// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Codec.h"

#pragma warning( push )
#pragma warning( disable: 4127 )
#include <QtCore/qtextcodec.h>
#pragma warning( pop )

// -------------------------------------------------------------------------
// Name: tools::SetCodec
// Created: ABR 2012-07-17
// -------------------------------------------------------------------------
void tools::SetCodec()
{
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
    QTextCodec::setCodecForTr( QTextCodec::codecForName( "UTF-8" ) );
}
