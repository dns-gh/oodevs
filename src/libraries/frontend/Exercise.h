// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Exercise_h_
#define __Exercise_h_

#include "Messages.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
}

namespace frontend
{
    class Terrain;
    class Dataset;
    class PhysicalModel;

// =============================================================================
/** @class  Exercise
    @brief  Exercise
*/
// Created: SBO 2007-01-29
// =============================================================================
class Exercise : public kernel::Extension_ABC
               , public kernel::Updatable_ABC< ASN1T_MsgExerciseUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             Exercise( const ASN1T_MsgExerciseCreation& message, kernel::Controllers& controllers
                     , const kernel::StringResolver< Terrain >& terrainResolver
                     , const kernel::StringResolver< Dataset >& datasetResolver
                     , const kernel::StringResolver< PhysicalModel >& physicalResolver );
    virtual ~Exercise();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Exercise( const Exercise& );            //!< Copy constructor
    Exercise& operator=( const Exercise& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgExerciseUpdate& message );
    void Update( const ASN1T_Exercise& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller&                            controller_;
    const kernel::StringResolver< Terrain >&       terrainResolver_;
    const kernel::StringResolver< Dataset >&       datasetResolver_;
    const kernel::StringResolver< PhysicalModel >& physicalResolver_;
    QString                                        name_;
    kernel::SafePointer< Terrain >                 terrain_;
    kernel::SafePointer< Dataset >                 dataset_;
    kernel::SafePointer< PhysicalModel >           physical_;
    //@}
};

}

#endif // __Exercise_h_
