// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterAutomat_h_
#define __MissionParameterAutomat_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{
    class Automat;
}

namespace plugins
{
namespace bml
{

// =============================================================================
/** @class  MissionParameterAutomat
    @brief  MissionParameterAutomat
*/
// Created: SBO 2008-06-02
// =============================================================================
class MissionParameterAutomat : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterAutomat( const kernel::OrderParameter& type, const dispatcher::Automat& automat );
    virtual ~MissionParameterAutomat();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ASN1T_MissionParameter& parameter ) const;
    virtual void Clean( ASN1T_MissionParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterAutomat( const MissionParameterAutomat& );            //!< Copy constructor
    MissionParameterAutomat& operator=( const MissionParameterAutomat& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Automat& automat_;
    //@}
};

}
}

#endif // __MissionParameterAutomat_h_
