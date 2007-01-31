// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DataManager_h_
#define __DataManager_h_

#include "ModelsContainer.h"
#include "Terrain.h"
#include "Dataset.h"

namespace master
{
class Master;
class Config;
class Exercise;

// =============================================================================
/** @class  DataManager
    @brief  DataManager
*/
// Created: NLD 2007-01-29
// =============================================================================
class DataManager
{
public:
    //! @name Constructors/Destructor
    //@{
             DataManager( const Master& master, const Config& config );
    virtual ~DataManager();
    //@}

    //! @name Accessors
    //@{
    const ModelsContainer< std::string, Terrain  >& GetTerrains () const;
    const ModelsContainer< std::string, Dataset  >& GetDatasets () const;
    const ModelsContainer< std::string, Exercise >& GetExercises() const;
    //@}

    //! @name Operations
    //@{
    void Send( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DataManager( const DataManager& );            //!< Copy constructor
    DataManager& operator=( const DataManager& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeTerrains ( const Config& config );
    void InitializeDatasets ( const Config& config );
    void InitializeExercises( const Config& config );    
    //@}

private:
    //! @name Member data
    //@{
    const Master& master_;

    ModelsContainer< std::string, Terrain  > terrains_;
    ModelsContainer< std::string, Dataset  > datasets_;
    ModelsContainer< std::string, Exercise > exercises_;
    //@}
};

}

#endif // __DataManager_h_
