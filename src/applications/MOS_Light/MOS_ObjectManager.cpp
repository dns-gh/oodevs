//*****************************************************************************
// Created: JVT 02-10-24
//*****************************************************************************

#include "MOS_Pch.h"

#include "MOS_ObjectManager.h"

#include "MOS_Object.h"
#include "MOS_Tri.h"
#include "MOS_Line.h"
#include "MOS_Node.h"

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager constructor
// Created: JVT 02-10-24
//-----------------------------------------------------------------------------
MOS_ObjectManager::MOS_ObjectManager()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager destructor
// Created: JVT 02-10-24
//-----------------------------------------------------------------------------
MOS_ObjectManager::~MOS_ObjectManager()
{
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::Initialize
// Created: FBD 02-11-21
// Last modified: JVT 03-12-11
//-----------------------------------------------------------------------------
void MOS_ObjectManager::Initialize( MT_InputArchive_ABC& archive )
{
    std::string strBuf;



    archive.ReadField( "Objects", strBuf ) && Read      ( strBuf );
    archive.ReadField( "Node", strBuf )    && ReadNodes ( strBuf );
    archive.ReadField( "Line", strBuf )    && ReadLines ( strBuf );
    archive.ReadField( "Tri", strBuf )     && ReadTris  ( strBuf );
    archive.ReadField( "Shape", strBuf )   && ReadShapes( strBuf );
}

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::Terminate
// Created: FBD 02-11-21
//-----------------------------------------------------------------------------
void MOS_ObjectManager::Terminate()
{
    DestroyObjects();
}

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::Read
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
int MOS_ObjectManager::Read( const std::string& strFile )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( strFile );
    
    uint nObjects = 0;
    archive.ReadField( "nObjects", nObjects );
    
    for( uint i = 0; i < nObjects; ++i )
    {
        int nTypeObject = 0;
        archive.ReadField( "nTypeObject", nTypeObject );
        MOS_Object* pObject = new MOS_Object( (E_TerrainObjectTypeID)nTypeObject );
        pObject->Read( archive );
        bool bOut = objectMap_.insert( std::make_pair( pObject->GetId(), pObject ) ).second;
        assert( bOut );
        static_cast< void >( bOut );
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadShapes
// Created: FBD 03-04-03
//-----------------------------------------------------------------------------
int MOS_ObjectManager::ReadShapes( const std::string& sName )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( sName );

    // Objects
    uint nNbrObjects = 0;
    archive.ReadField( "nNbrObjects", nNbrObjects );

    MT_LOG_INFO_MSG( MT_FormatString( "[Reading] %d Objects Shapes", nNbrObjects ).c_str() );
    for( uint i = 0 ; i < nNbrObjects; ++i )
    {
        uint32 nIdObject = 0;
        archive.ReadField( "nIdObject", nIdObject );
        MOS_Object* pObject = &GetObjectFromId( nIdObject );
        assert( pObject );
        pObject->ReadGeom( archive );
    }
    return 0;
   
}

//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadTris
// Created: FBD 03-04-03
//-----------------------------------------------------------------------------
int MOS_ObjectManager::ReadTris( const std::string& sName )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( sName );

    // Tris
    uint nTris = 0;
    archive.ReadField( "nTris", nTris );
    triVector_.reserve( nTris );
    MT_LOG_INFO_MSG( MT_FormatString( "[Reading] %d Triangles", nTris ).c_str() );

    for ( uint i = 0 ; i < nTris ; ++i )
    {
        MOS_Tri tri;
        tri.Read( archive );
        triVector_.push_back( tri );
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadLines
// Created: FBD 03-04-03
//-----------------------------------------------------------------------------
int MOS_ObjectManager::ReadLines( const std::string& sName )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( sName );

    // Lines
    uint nLines = 0;
    archive.ReadField( "nLines", nLines );
    lineVector_.reserve( nLines );

    MT_LOG_INFO_MSG( MT_FormatString( "[Reading] %d Lines", nLines ).c_str() );
    
    for ( uint i = 0 ; i < nLines ; ++i )
    {
        MOS_Line line;
        line.Read( archive );
        lineVector_.push_back( line );
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadNodes
// Created: FBD 03-04-03
//-----------------------------------------------------------------------------
int MOS_ObjectManager::ReadNodes( const std::string& sName )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( sName );

    // Nodes
    uint nNodes = 0;
    archive.ReadField( "nNodes", nNodes );
    MT_LOG_INFO_MSG( MT_FormatString( "[Reading] %d Nodes", nNodes ).c_str() );
    nodeVector_.reserve( nNodes );
    for( uint i = 0 ; i < nNodes ; ++i )
    {
        MOS_Node node;
        node.Read( archive );
        nodeVector_.push_back( node );
    }
    return 0;
}
