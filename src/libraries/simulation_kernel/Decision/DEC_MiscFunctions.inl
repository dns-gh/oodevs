// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Functions/DEC_MiscFunctions.inl $
// $Author: Nld $
// $Modtime: 9/03/05 16:17 $
// $Revision: 2 $
// $Workfile: DEC_MiscFunctions.inl $
//
// *****************************************************************************

#include "Entities/Orders/MIL_Report.h"
#include "Entities/Orders/MIL_MissionParameter_ABC.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "MT_Tools/MT_FormatString.h"

//-----------------------------------------------------------------------------
// Name: DEC_PopulationFunctions::Trace
// Created: AHC 2009-07-30
//-----------------------------------------------------------------------------
template <typename T>
void DEC_MiscFunctions::Trace( const T& caller, const std::string& message )
{
    try
    {
        client::Trace msg;
        MIL_AgentServer::GetWorkspace().GetEntityManager().SetToTasker( *msg().mutable_source(), caller.GetID() );
        *msg().mutable_message() = message.c_str();
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
    catch( std::exception& ) {}
}

//-----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::Debug
// Created: AHC 2009-07-30
//-----------------------------------------------------------------------------
template <typename T>
void DEC_MiscFunctions::Debug( const T& caller, const std::string& callerType, const std::string& message )
{
    if( !MIL_AgentServer::GetWorkspace().GetConfig().UseDecDebug() )
        return;
    MT_LOG_INFO_MSG( MT_FormatString( "%s %d says : [%s]", callerType.c_str(), caller.GetID(), message.c_str() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DebugDrawPoints
// Created: AHC 2009-07-30
// -----------------------------------------------------------------------------
template <typename T>
void DEC_MiscFunctions::DebugDrawPoints(const T& caller, std::vector< boost::shared_ptr< MT_Vector2D > > points )
{
    client::DebugPoints message;
    MIL_AgentServer::GetWorkspace().GetEntityManager().SetToTasker( *message().mutable_source(), caller.GetID() );
    NET_ASN_Tools::WriteCoordinates( points, *message().mutable_coordinates() );
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_MiscFunctions::DebugDrawPoint
// Created: NLD 2005-03-22
// Created: AHC 2009-07-30-----------------------------------------------
template <typename T>
void DEC_MiscFunctions::DebugDrawPoint( const T& caller, const MT_Vector2D* pPoint )
{
    assert( pPoint );
    client::DebugPoints message;
    MIL_AgentServer::GetWorkspace().GetEntityManager().SetToTasker( *message().mutable_source(), caller.GetID() );
    NET_ASN_Tools::WritePoint( *pPoint, *message().mutable_coordinates()->add_elem() );
    message.Send( NET_Publisher_ABC::Publisher() );
}
