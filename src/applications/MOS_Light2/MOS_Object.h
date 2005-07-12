//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Object.h $
// $Author: Ape $
// $Modtime: 20/07/04 12:06 $
// $Revision: 5 $
// $Workfile: MOS_Object.h $
//
//*****************************************************************************

#ifndef __MOS_Object_h_
#define __MOS_Object_h_

class MOS_Patch;
class MOS_ShapePatch;


// =============================================================================
/** @class  MOS_Object
    @brief  A non-dynamic map object, i.e. a forest, city, road, bridge...
    @par    MOS_Object are managed by the MOS_ObjectManager.

            A MOS_Object owns a MOS_ShapePatch for each patch is spans on and 
            thus has it's geometry split up between patches. This is used to 
            speed up rendering, as a patch is only drawn if it's on screen. For
            example, thanks to this, a large forest does not have to be drawn
            whole if only a small portion of it is on screen.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Object
{
    MT_COPYNOTALLOWED( MOS_Object )
    friend class MOS_GLTool;
    
public:
    //-------------------------------------------------------------------------
    /** @name Types*/
    //-------------------------------------------------------------------------
    //@{
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
    MT_Float                GetValue() const;
    const std::string&      GetName() const;

    bool                    IsPoint() const;
    bool                    IsLine() const;
    bool                    IsArea() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name IN / OUT*/
    //-------------------------------------------------------------------------
    //@{
    void Read( MT_InputArchive_ABC& archive );
    void ReadGeom( MT_InputArchive_ABC& archive );
    //@}

private:
    typedef std::vector< MOS_ShapePatch* >      T_ShapePatchVector;
    typedef T_ShapePatchVector::iterator        IT_ShapePatchVector;
    typedef T_ShapePatchVector::const_iterator  CIT_ShapePatchVector;

private:
    uint32                nId_;
    E_TerrainObjectTypeID nType_;
    E_ShapeTypeID         nTypeShape_;

    std::string           sName_;
    MT_Vector2D           vPosText_;
    MT_Float              rValue_;
    T_ShapePatchVector    shapePatchs_;
};


#ifdef MOS_USE_INLINE
#	include "MOS_Object.inl"
#endif


#endif // __MOS_Object_h_