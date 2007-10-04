// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CreateTerrain_h_
#define __CreateTerrain_h_

#include <qprocess.h>

namespace tools 
{
    class GeneralConfig;
}

// =============================================================================
/** @class  CreateTerrain
    @brief  CreateTerrain
*/
// Created: AGE 2007-10-04
// =============================================================================
class CreateTerrain : public QProcess
{

public:
    //! @name Constructors/Destructor
    //@{
             CreateTerrain( QObject* parent, const tools::GeneralConfig& config, const std::string& name );
    virtual ~CreateTerrain();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreateTerrain( const CreateTerrain& );            //!< Copy constructor
    CreateTerrain& operator=( const CreateTerrain& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __CreateTerrain_h_
