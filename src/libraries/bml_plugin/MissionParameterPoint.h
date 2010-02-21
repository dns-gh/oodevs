// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterPoint_h_
#define __MissionParameterPoint_h_

#include "MissionParameter_ABC.h"

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace bml
{
    class Point;

// =============================================================================
/** @class  MissionParameterPoint
    @brief  MissionParameterPoint
*/
// Created: SBO 2008-06-02
// =============================================================================
class MissionParameterPoint : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterPoint( xml::xistream& xis, const kernel::OrderParameter& type );
    virtual ~MissionParameterPoint();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( MsgMissionParameter& parameter ) const;
    virtual void Clean( MsgMissionParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterPoint( const MissionParameterPoint& );            //!< Copy constructor
    MissionParameterPoint& operator=( const MissionParameterPoint& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Point > point_;
    //@}
};

}
}

#endif // __MissionParameterPoint_h_
