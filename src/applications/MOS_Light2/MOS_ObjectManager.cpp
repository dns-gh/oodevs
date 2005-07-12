//*****************************************************************************
// Created: JVT 02-10-24
//*****************************************************************************

#include "MOS_Pch.h"
#include "MOS_ObjectManager.h"

#include "MOS_Object.h"


#ifndef  MOS_USE_INLINE 
#   include "MOS_ObjectManager.inl"
#endif


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
    for( IT_ObjectPtrMap itObject = objectMap_.begin(); itObject != objectMap_.end(); ++itObject )
        delete itObject->second;
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::Initialize
// Created: FBD 02-11-21
// Last modified: JVT 03-12-11
//-----------------------------------------------------------------------------
void MOS_ObjectManager::Initialize( MT_InputArchive_ABC& archive )
{
    std::string strFilename;

    archive.ReadField( "Objects", strFilename );
    this->ReadObjects( strFilename );

    archive.ReadField( "Node", strFilename );
    this->ReadNodes( strFilename );

    archive.ReadField( "Line", strFilename );
    this->ReadLines( strFilename );

    archive.ReadField( "Tri", strFilename );
    this->ReadTris( strFilename );

    archive.ReadField( "Shape", strFilename );
    this->ReadShapes( strFilename );
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadObjects
// Created: FBD 02-11-15
//-----------------------------------------------------------------------------
void MOS_ObjectManager::ReadObjects( const std::string& strFilename )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( strFilename );
    
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
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadShapes
// Created: FBD 03-04-03
//-----------------------------------------------------------------------------
void MOS_ObjectManager::ReadShapes( const std::string& strFilename )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( strFilename );

    uint nNbrObjects = 0;
    archive.ReadField( "nNbrObjects", nNbrObjects );

    for( uint i = 0; i < nNbrObjects; ++i )
    {
        uint32 nIdObject = 0;
        archive.ReadField( "nIdObject", nIdObject );
        MOS_Object* pObject = &GetObjectFromId( nIdObject );
        assert( pObject );
        pObject->ReadGeom( archive );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadTris
// Created: FBD 03-04-03
//-----------------------------------------------------------------------------
void MOS_ObjectManager::ReadTris( const std::string& strFilename )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( strFilename );

    uint nTris = 0;
    archive.ReadField( "nTris", nTris );
    triVector_.reserve( nTris );

    for ( uint i = 0; i < nTris; ++i )
    {
        MOS_Tri tri;
        tri.Read( archive );
        triVector_.push_back( tri );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadLines
// Created: FBD 03-04-03
//-----------------------------------------------------------------------------
void MOS_ObjectManager::ReadLines( const std::string& strFilename )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( strFilename );

    uint nLines = 0;
    archive.ReadField( "nLines", nLines );
    lineVector_.reserve( nLines );
    
    for ( uint i = 0; i < nLines; ++i )
    {
        MOS_Line line;
        line.Read( archive );
        lineVector_.push_back( line );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::ReadNodes
// Created: FBD 03-04-03
//-----------------------------------------------------------------------------
void MOS_ObjectManager::ReadNodes( const std::string& strFilename )
{
    MT_FlatBinaryInputArchive archive;
    archive.Open( strFilename );

    uint nNodes = 0;
    archive.ReadField( "nNodes", nNodes );
    nodeVector_.reserve( nNodes );

    for( uint i = 0; i < nNodes; ++i )
    {
        MOS_Node node;
        node.Read( archive );
        nodeVector_.push_back( node );
    }
}
