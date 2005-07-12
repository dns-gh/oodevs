//*****************************************************************************
// Name   : MOS_Node.h
// Created: FBD 02-09-19
//*****************************************************************************

#ifndef __MOS_Node_h_
#define __MOS_Node_h_

#include "MOS_Types.h"

class MOS_Object_ABC;
class MOS_NodeLink;
class MOS_DynaObject_ABC;

//=============================================================================
// Created: FBD 02-09-19
//=============================================================================
class MOS_Node
{
        
public:

    MOS_Node();
    virtual ~MOS_Node();
    MOS_Node( const MOS_Node& rhs );
    MOS_Node& operator = ( const MOS_Node& rhs );

    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< MOS_Node*, MOS_NodeLink* > T_NodeLinkMap;
    typedef T_NodeLinkMap::iterator              IT_NodeLinkMap;

    typedef std::vector< MOS_NodeLink* >        T_NodeLinkVector;
    typedef const T_NodeLinkVector              CT_NodeLinkVector;
    typedef T_NodeLinkVector::iterator          IT_NodeLinkVector;
    typedef T_NodeLinkVector::const_iterator    CIT_NodeLinkVector;
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D&  GetPos() const;
    MT_Vector2D&        GetPos();
    bool                IsInside( const MT_Vector2D& vPos, MT_Float rWeldValue = 0.0 ) const;

    //@}

    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    int Read     ( MT_InputArchive_ABC& );
    //@}


private:

    MT_Vector2D         vPos_;
};

typedef std::vector< MOS_Node >           T_NodeVector;
typedef const T_NodeVector              CT_NodeVector;
typedef T_NodeVector::iterator          IT_NodeVector;
typedef T_NodeVector::const_iterator    CIT_NodeVector;

#   include "MOS_Node.inl"

#endif // __MOS_Node_h_
