//*****************************************************************************
// Created: JVT 02-10-24
//*****************************************************************************

#ifndef __MOS_ObjectManager_h_
#define __MOS_ObjectManager_h_

#include "MOS_Tri.h"
#include "MOS_Line.h"
#include "MOS_Node.h"

class MOS_Object;


// =============================================================================
/** @class  MOS_ObjectManager
    @brief  Manages all the non-dynamic map objects, i.e. forests, cities, roads...
    @par    The ObjectManager owns all the objects and all the underlying geometry
            (lines, points and triangles). The objects themselves own a MOS_ShapePatch
            for each MOS_Patch (a MOS_Patch being a rectangular subdivision of the map)
            the object spands on. Thoses MOS_ShapePatch in turn hold pointers to
            the geometry elements that compose them. Finally, each MOS_Patch has
            pointers to all the MOS_ShapePatch that are on it.
*/
// Created: APE 2004-07-19
// =============================================================================
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
    //@}

    //-------------------------------------------------------------------------
    /** @name Objects ID */
    //-------------------------------------------------------------------------
    //@{
    MOS_Object&  GetObjectFromId( uint32 nObjectId );
    MOS_Node* GetNodeFromIdx( uint nIdx );
    MOS_Line* GetLineFromIdx( uint nIdx );
    MOS_Tri* GetTriFromIdx( uint nIdx );
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    void ReadObjects( const std::string& strFilename );
    void ReadShapes( const std::string& strFilename );
    void ReadTris( const std::string& strFilename );
    void ReadLines( const std::string& strFilename );
    void ReadNodes( const std::string& strFilename );
    //@}

private:
    T_ObjectPtrMap         objectMap_;
    
    MOS_Tri::T_TriVector        triVector_;
    MOS_Line::T_LineVector      lineVector_;
    MOS_Node::T_NodeVector      nodeVector_; 
};

#ifdef MOS_USE_INLINE
#   include "MOS_ObjectManager.inl"
#endif

#endif // __MOS_ObjectManager_h_