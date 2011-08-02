// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GraphicPreferences_h_
#define __GraphicPreferences_h_

#include <spatialcontainer/TerrainData.h>
#include <graphics/GraphicSetup_ABC.h>

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class TerrainPreference;

// =============================================================================
/** @class  GraphicPreferences
    @brief  Graphic preferences
*/
// Created: SBO 2006-04-04
// =============================================================================
class GraphicPreferences : public GraphicSetup_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GraphicPreferences( kernel::Controllers& controllers );
    virtual ~GraphicPreferences();
    //@}

    //! @name Operations
    //@{
    void SetAlpha( float a );

    void Display( QWidget* parent ) const;

    virtual void SetupLineGraphics  ( const Data_ABC* pData );
    virtual void SetupLineGraphics  ( unsigned int offset );
    virtual void SetupBorderGraphics( const Data_ABC* pData );
    virtual void SetupAreaGraphics  ( const Data_ABC* pData );

    void Commit();
    void Revert();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GraphicPreferences( const GraphicPreferences& );            //!< Copy constructor
    GraphicPreferences& operator=( const GraphicPreferences& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadTerrainPreference( xml::xistream& xis );
    void Save() const;
    void Save( xml::xostream& xos, const TerrainPreference& preference ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< TerrainData, TerrainPreference* > T_TerrainPreferences;
    typedef T_TerrainPreferences::const_iterator      CIT_TerrainPreferences;
    typedef std::vector< TerrainPreference* >           T_Displays;
    typedef T_Displays::const_iterator                CIT_Displays;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_TerrainPreferences terrainPrefs_;
    T_Displays displays_;
    float alpha_;
    //@}
};

}

#endif // __GraphicPreferences_h_
