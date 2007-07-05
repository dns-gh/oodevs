// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ReplayModel.h"
#include "tools/OutputBinaryWrapper.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: ReplayModel constructor
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
ReplayModel::ReplayModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayModel destructor
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
ReplayModel::~ReplayModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayModel::OnReceive
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void ReplayModel::OnReceive( const ASN1T_MsgsSimToClient& asnMsg )
{
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsSimToClient asnMsgCtrl( asnPEREncodeBuffer, const_cast< ASN1T_MsgsSimToClient& >( asnMsg ) );
    asnMsgCtrl.Encode();

    std::ofstream& archive = GetArchive( asnMsg.msg.t );
    tools::OutputBinaryWrapper output(archive  );
    output << asnPEREncodeBuffer.GetMsgLen();
    output.Write( (const char*)asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );
    archive.flush();
}
    
// -----------------------------------------------------------------------------
// Name: ReplayModel::StartSynchronisation
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void ReplayModel::StartSynchronisation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayModel::EndSynchronisation
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
void ReplayModel::EndSynchronisation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayModel::GetArchive
// Created: AGE 2007-07-05
// -----------------------------------------------------------------------------
std::ofstream& ReplayModel::GetArchive( unsigned int type )
{
    std::ofstream*& archive = archives_[ type ];
    if( ! archive )
    {
        std::string path = "dump/" + boost::lexical_cast< std::string >( type );
        archive = new std::ofstream( path.c_str() );
    }
    return *archive;
}
