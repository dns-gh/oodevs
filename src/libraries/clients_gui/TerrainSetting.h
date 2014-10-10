// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef TerrainSetting_h
#define TerrainSetting_h

namespace gui
{

// =============================================================================
/** @class  TerrainSetting
    @brief  TerrainSetting
*/
// Created: ABR 2014-07-07
// =============================================================================
struct TerrainSetting
{
    //! @name Constructors/Destructor
    //@{
    explicit TerrainSetting( const std::string& type )
        : type_( type )
    {
        // NOTHING
    }
    //@}

    //! @name Member data
    //@{
    std::string type_;
    QString name_;
    QString category_;
    bool shown_;
    float lineWidth_;
    QColor color_;
    //@}
};

} //! namespace gui

#endif // TerrainSetting_h
