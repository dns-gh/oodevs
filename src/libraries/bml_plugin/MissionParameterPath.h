// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterPath_h_
#define __MissionParameterPath_h_

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
/** @class  MissionParameterPath
    @brief  MissionParameterPath
*/
// Created: SBO 2008-06-02
// =============================================================================
class MissionParameterPath : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterPath( xml::xistream& xis, const kernel::OrderParameter& type );
    virtual ~MissionParameterPath();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ASN1T_MissionParameter& parameter ) const;
    virtual void Clean( ASN1T_MissionParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterPath( const MissionParameterPath& );            //!< Copy constructor
    MissionParameterPath& operator=( const MissionParameterPath& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< PointList > points_;
    //@}
};

}
}

#endif // __MissionParameterPath_h_
