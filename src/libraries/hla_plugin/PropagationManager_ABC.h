// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_PropagationManager_ABC_h
#define plugins_hla_PropagationManager_ABC_h

#include "ObjectListener_ABC.h"

namespace plugins
{
namespace hla
{

class PropagationManager_ABC
{
public:
    virtual ~PropagationManager_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void saveDataFile( const std::string& identifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
            int col, int lig, double xll, double yll, double cellsize ) = 0;
    //@}

};

} // namespace hla
} // namespace plugins
#endif // plugins_hla_PropagationManager_ABC_h