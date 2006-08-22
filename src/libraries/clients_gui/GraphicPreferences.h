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

#include "pathfind/TerrainData.h"
#include "graphics/GraphicSetup_ABC.h"

namespace xml { class xistream; class xostream; };

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
             GraphicPreferences();
    virtual ~GraphicPreferences();
    //@}

    //! @name Operations
    //@{
    void Display( QWidget* parent ) const;

    virtual void SetupLineGraphics  ( const Data_ABC* pData );
    virtual void SetupBorderGraphics( const Data_ABC* pData );
    virtual void SetupAreaGraphics  ( const Data_ABC* pData );

    void Commit();
    void Revert();
    //@}

    //! @name Accessors
    //@{
//    const GraphicPreference_ABC& GetPreference( const TerrainData& terrain ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GraphicPreferences( const GraphicPreferences& );            //!< Copy constructor
    GraphicPreferences& operator=( const GraphicPreferences& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void InitializeTerrainPreferences();
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
    T_TerrainPreferences terrainPrefs_;
    T_Displays displays_;
    //@}
};

}

#endif // __GraphicPreferences_h_
