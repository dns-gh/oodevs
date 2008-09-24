// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterLimit_h_
#define __MissionParameterLimit_h_

#include "MissionParameter_ABC.h"

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace bml
{
    class PointList;

// =============================================================================
/** @class  MissionParameterLimit
    @brief  MissionParameterLimit
*/
// Created: SBO 2008-05-22
// =============================================================================
class MissionParameterLimit : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterLimit( xml::xistream& xis, const kernel::OrderParameter& type );
    virtual ~MissionParameterLimit();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ASN1T_MissionParameter& parameter ) const;
    virtual void Clean( ASN1T_MissionParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterLimit( const MissionParameterLimit& );            //!< Copy constructor
    MissionParameterLimit& operator=( const MissionParameterLimit& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< PointList > points_;
    //@}
};

}
}

#endif // __MissionParameterLimit_h_
