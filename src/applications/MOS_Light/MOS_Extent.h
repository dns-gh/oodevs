//*****************************************************************************
// Name   : MOS_Extent.h
// Created: FBD 02-04-19
//*****************************************************************************

#ifndef __MOS_Extent_h_
#define __MOS_Extent_h_

#include "MOS_Types.h"

#include "MT_Rect.h"

class MT_OutputArchive_ABC;

//=============================================================================
// Created: FBD 02-04-19
//=============================================================================
class MOS_Extent
{
    MT_COPYNOTALLOWED( MOS_Extent );

public:
    explicit MOS_Extent();
    ~MOS_Extent();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const std::string&  GetTopDMSAsString     () const;
	const std::string&  GetBottomDMSAsString  () const;
	const std::string&  GetLeftDMSAsString    () const;
	const std::string&  GetRightDMSAsString   () const;
    const std::string&  GetWidthPixelAsString () const;
    const std::string&  GetHeightPixelAsString() const;

    const MT_Rect&     GetRectDMS() const;
    const MT_Rect&     GetRect   () const;
                        
    MT_Float            GetMeterPerPixel() const;
    //@}
    
    //-------------------------------------------------------------------------
    /** @name IN/OUT */
    //-------------------------------------------------------------------------
    //@{
    int Read( MT_InputArchive_ABC& archive );
    //@}
    
private:        
    MT_Rect    rectDMS_;
    MT_Rect    rect_;

    std::string strDMSTop_;
    std::string strDMSBottom_;
    std::string strDMSLeft_;
    std::string strDMSRight_;

    std::string strWidthPixel_;
    std::string strHeightPixel_;
    
    MT_Float rMeterPerPixel_;
};

# include "MOS_Extent.inl"

#endif // __MOS_Extent_h_
