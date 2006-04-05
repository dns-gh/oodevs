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

class QTabWidget;
class TerrainPreference;

// =============================================================================
/** @class  GraphicPreferences
    @brief  Graphic preferences
*/
// Created: SBO 2006-04-04
// =============================================================================
class GraphicPreferences
{

public:
    //! @name Constructors/Destructor
    //@{
             GraphicPreferences();
    virtual ~GraphicPreferences();
    //@}

    //! @name Operations
    //@{
    void AddPanels( QTabWidget* parent ) const;

    void ReadPreferences ();
    void WritePreferences();
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
    void AddTerrainPanel( QTabWidget* parent ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< TerrainData, TerrainPreference* > T_TerrainPreferences;
    typedef T_TerrainPreferences::iterator             IT_TerrainPreferences;
    typedef T_TerrainPreferences::const_iterator      CIT_TerrainPreferences;
    //@}

private:
    //! @name Member data
    //@{
    T_TerrainPreferences terrainPrefs_;
    //@}
};

#endif // __GraphicPreferences_h_
