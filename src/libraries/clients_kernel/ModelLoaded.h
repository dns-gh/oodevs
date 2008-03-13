// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelLoaded_h_
#define __ModelLoaded_h_

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{

// =============================================================================
/** @class  ModelLoaded
    @brief  Model loaded "event"
*/
// Created: SBO 2006-05-24
// =============================================================================
class ModelLoaded
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ModelLoaded( const tools::ExerciseConfig& config );
    virtual ~ModelLoaded();
    //@}

    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ModelLoaded( const ModelLoaded& );            //!< Copy constructor
    ModelLoaded& operator=( const ModelLoaded& ); //!< Assignement operator
    //@}
};

class ModelUnLoaded
{
public:
             ModelUnLoaded() {}
    virtual ~ModelUnLoaded() {}
};

}

#endif // __ModelLoaded_h_
