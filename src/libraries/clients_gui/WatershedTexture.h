// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef WatershedTexture_h
#define WatershedTexture_h

#include <boost/noncopyable.hpp>

namespace kernel
{
    class DetectionMap;
    class Options;
}

namespace gui
{

// =============================================================================
/** @class  WatershedTexture
    @brief  WatershedTexture
*/
// Created: ABR 2014-07-02
// =============================================================================
class WatershedTexture : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             WatershedTexture( const kernel::DetectionMap& map );
    virtual ~WatershedTexture();
    //@}

    //! @name Operations
    //@{
    void Load( const kernel::Options& options );
    unsigned int CreateTexture();
    //@}

private:
    //! @name Helpers
    //@{
    void Reset();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& map_;
    unsigned int texture_;
    unsigned short height_;
    bool inverse_;
    QColor color_;
    float alpha_;
    //@}
};

} //! namespace gui

#endif // WatershedTexture_h
