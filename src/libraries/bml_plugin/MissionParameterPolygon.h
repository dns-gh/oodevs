// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterPolygon_h_
#define __MissionParameterPolygon_h_

#include "MissionParameter_ABC.h"

namespace xml
{
    class xistream;
}

namespace bml
{
    class PointList;

// =============================================================================
/** @class  MissionParameterPolygon
    @brief  MissionParameterPolygon
*/
// Created: SBO 2008-05-22
// =============================================================================
class MissionParameterPolygon : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterPolygon( xml::xistream& xis, const kernel::OrderParameter& type );
    virtual ~MissionParameterPolygon();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ASN1T_MissionParameter& parameter ) const;
    virtual void Clean( ASN1T_MissionParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterPolygon( const MissionParameterPolygon& );            //!< Copy constructor
    MissionParameterPolygon& operator=( const MissionParameterPolygon& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< PointList > points_;
    //@}
};

}

#endif // __MissionParameterPolygon_h_
