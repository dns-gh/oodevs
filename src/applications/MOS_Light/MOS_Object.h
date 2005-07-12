//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Object.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_Object.h $
//
//*****************************************************************************

#ifndef __MOS_Object_h_
#define __MOS_Object_h_

#include "MOS_Types.h"
#include "MOS_ShapePatch.h"

class MOS_Patch;

//*****************************************************************************
// Created: FBD 02-11-21
//*****************************************************************************
class MOS_Object
{
    MT_COPYNOTALLOWED( MOS_Object )

    friend class MOS_ShapePatch; // For Register/Unregister
    
public:
    //-------------------------------------------------------------------------
    /** @name Types*/
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< MOS_Patch*, MOS_ShapePatch* >     T_ShapePatchPtrMap;
    typedef const T_ShapePatchPtrMap                    CT_ShapePatchPtrMap;
    typedef T_ShapePatchPtrMap::iterator                IT_ShapePatchPtrMap;
    typedef T_ShapePatchPtrMap::const_iterator          CIT_ShapePatchPtrMap;

    typedef std::map< MOS_Patch*, MOS_ShapePatch >     T_ShapePatchMap;
    typedef const T_ShapePatchMap                    CT_ShapePatchMap;
    typedef T_ShapePatchMap::iterator                IT_ShapePatchMap;
    typedef T_ShapePatchMap::const_iterator          CIT_ShapePatchMap;
    //@}


public:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< MOS_ShapePatch* >  T_ShapePatchPtrVector;
    typedef T_ShapePatchPtrVector::iterator IT_ShapePatchPtrVector;
    //@}
    
public:
    explicit MOS_Object( E_TerrainObjectTypeID nType );
    virtual ~MOS_Object();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    E_TerrainObjectTypeID   GetType() const;
    uint32                  GetId  () const;
    bool                    IsItPoint() const;
    bool                    IsItLine() const;
    bool                    IsItArea() const;
    MT_Float                GetValue() const;
    const std::string&      GetName() const;
    void                    DrawObjectsName( MT_Float rDistPercent );
    uint                    GetFrame() const;
    void                    SetFrame( uint nFrame );
    const MT_Vector2D&      GetPosText() const;
    CT_ShapePatchMap&       GetListShapePatchs() const;
    void                    DeleteAllShapePatchs();
    //@}
    
    //-------------------------------------------------------------------------
    /** @name IN / OUT*/
    //-------------------------------------------------------------------------
    //@{
    void Read( MT_InputArchive_ABC& archive );
    void ReadGeom( MT_InputArchive_ABC& archive );
    //@}

private:
    uint32                nId_;
    E_TerrainObjectTypeID nType_;
    E_ShapeTypeID         nTypeShape_;

    std::string           sName_;
    MT_Vector2D           vPosText_;
    MT_Float              rValue_;
    T_ShapePatchMap       shapePatchMap_;
    uint                  nFrame_;    
};

#ifdef MOS_USE_INLINE
#	include "MOS_Object.inl"
#endif


#endif // __MOS_Object_h_