//*****************************************************************************
// Name   : MOS_Extent.h
// Created: FBD 02-04-19
//*****************************************************************************

#ifndef __MOS_Extent_h_
#define __MOS_Extent_h_


// =============================================================================
/** @class  MOS_Extent
    @brief  Map size and resolution data.
*/
// Created: APE 2004-07-19
// =============================================================================
class MOS_Extent
{
    MT_COPYNOTALLOWED( MOS_Extent );

public:
     MOS_Extent();
    ~MOS_Extent();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Rect&     GetRectDMS() const;
    const MT_Rect&     GetRect   () const;
                        
    MT_Float           GetMeterPerPixel() const;
    //@}
    
    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    void Read( MT_InputArchive_ABC& archive );
    //@}
    
private:        
    MT_Rect  rectDMS_;
    MT_Rect  rect_;
    MT_Float rMeterPerPixel_;
};


#ifdef  MOS_USE_INLINE
#   include "MOS_Extent.inl"
#endif  // ! _DEBUG

#endif // __MOS_Extent_h_
