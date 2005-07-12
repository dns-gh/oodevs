//*****************************************************************************
// Name   : MOS_Node.h
// Created: FBD 02-09-19
//*****************************************************************************

#ifndef __MOS_Node_h_
#define __MOS_Node_h_


// =============================================================================
/** @class  MOS_Node
    @brief  Geometry element of a map object.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Node
{
public:
    typedef std::vector< MOS_Node >   T_NodeVector;
    typedef T_NodeVector::iterator    IT_NodeVector;

public:
     MOS_Node();
    ~MOS_Node();

   //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D&  GetPos() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    void Read( MT_InputArchive_ABC& );
    //@}

private:
    MT_Vector2D vPos_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_Node.inl"
#endif

#endif // __MOS_Node_h_
