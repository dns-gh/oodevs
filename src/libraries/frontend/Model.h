// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

namespace kernel
{
    class Controllers;
}

namespace frontend
{
    class ExercisesModel;
    class ExerciseFactory_ABC;
    class TerrainsModel;
    class TerrainFactory_ABC;
    class DatasetsModel;
    class DatasetFactory_ABC;
    class PhysicalModelsModel;
    class PhysicalModelFactory_ABC;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: SBO 2007-01-29
// =============================================================================
class Model
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Model( kernel::Controllers& controllers );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignment operator
    //@}

    //! @name Factories
    //@{
    TerrainFactory_ABC&       terrainFactory_;
    DatasetFactory_ABC&       datasetFactory_;
    PhysicalModelFactory_ABC& physicalModelFactory_;
    ExerciseFactory_ABC&      exerciseFactory_;
    //@}

public:
    //! @name Member data
    //@{
    TerrainsModel&       terrains_;
    DatasetsModel&       datasets_;
    PhysicalModelsModel& physicalModels_;
    ExercisesModel&      exercises_;
    //@}
};

}

#endif // __Model_h_
