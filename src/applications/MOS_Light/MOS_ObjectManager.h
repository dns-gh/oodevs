//*****************************************************************************
// Created: JVT 02-10-24
//*****************************************************************************

#ifndef __MOS_ObjectManager_h_
#define __MOS_ObjectManager_h_

#include "MOS_Types.h"
#include "MOS_Tri.h"
#include "MOS_Line.h"
#include "MOS_Node.h"

class MOS_Object;

//*****************************************************************************
// Created: JVT 02-10-24
// Last modified: JVT 03-12-11
//*****************************************************************************
class MOS_ObjectManager
{
    MT_COPYNOTALLOWED( MOS_ObjectManager )

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< uint32, MOS_Object* >     T_ObjectPtrMap;
    typedef const T_ObjectPtrMap                CT_ObjectPtrMap;
    typedef T_ObjectPtrMap::iterator            IT_ObjectPtrMap;
    typedef T_ObjectPtrMap::const_iterator      CIT_ObjectPtrMap;

    //@}

public:
     MOS_ObjectManager(); 
    ~MOS_ObjectManager();

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void Initialize( MT_InputArchive_ABC& );
    void Terminate ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Objects ID */
    //-------------------------------------------------------------------------
    //@{
    MOS_Object&  GetObjectFromId( uint32 nObjectIdx );
    //@}


    //-------------------------------------------------------------------------
    /** @name Node/Line/Triangle */
    //-------------------------------------------------------------------------
    //@{
    MOS_Node* GetNodeFromIdx( uint nIdx );
    MOS_Line* GetLineFromIdx( uint nIdx );
    MOS_Tri* GetTriFromIdx( uint nIdx );
    //@}



private:
    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    int Read( const std::string& strFile );
    int ReadShapes( const std::string& sName );
    int ReadTris( const std::string& sName );
    int ReadLines( const std::string& sName );
    int ReadNodes( const std::string& sName );
    //@}

    //-------------------------------------------------------------------------
    /** @name Objects */
    //-------------------------------------------------------------------------
    //@{
    void DestroyObjects();
    //@}

private:
    T_ObjectPtrMap            objectMap_;
    
    T_TriVector            triVector_;
    T_MOSLineVector        lineVector_;
    T_NodeVector           nodeVector_;
    
};

#include "MOS_ObjectManager.inl"

#endif // __MOS_ObjectManager_h_