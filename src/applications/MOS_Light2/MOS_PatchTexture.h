//*****************************************************************************
// Name   : MOS_PatchTexture.h
// Created: FBD 02-02-27
//*****************************************************************************

#ifndef __MOS_PatchTexture_h_
#define __MOS_PatchTexture_h_


// =============================================================================
/** @class  MOS_PatchTexture
    @brief  Map element that displays a generated heightmap.
    @par    Upon initialization, a pixmap and a GL display list using this
            pixmap is generated, making it easy to draw the heigthmap when
            necessary.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_PatchTexture
{
    MT_COPYNOTALLOWED( MOS_PatchTexture )
    friend class MOS_GLTool;

public:
     MOS_PatchTexture( const MT_Rect& rect );
    ~MOS_PatchTexture();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Rect& GetRect() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX */
    //-------------------------------------------------------------------------
    //@{
    void Initialize      ();
    void GenerateAltitude();
    void GenerateCouverture();  // Unused, kept for eventual future use.
    //@}

private:
    MT_Rect rect_;

    GLuint  nGLPatchAltitude_;
    GLuint  nGLPatchCouverture_;
};


#ifdef  MOS_USE_INLINE
#   include "MOS_PatchTexture.inl"
#endif

#endif // __MOS_PatchTexture_h_
