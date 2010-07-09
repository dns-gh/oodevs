// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterPhaseLine_h_
#define __MissionParameterPhaseLine_h_

#include "MissionParameter_ABC.h"
#include "protocol/protocol.h"

using namespace Common;

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
/** @class  MissionParameterPhaseLine
    @brief  MissionParameterPhaseLine
*/
// Created: SBO 2008-05-22
// =============================================================================
class MissionParameterPhaseLine : public MissionParameter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterPhaseLine( xml::xistream& xis, const kernel::OrderParameter& type, const std::string& functions );
    virtual ~MissionParameterPhaseLine();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( MsgMissionParameter& parameter ) const;
    virtual void Clean( MsgMissionParameter& parameter ) const;

    void Serialize( MsgLimaOrder& asn ) const;
    void Clean( MsgLimaOrder& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterPhaseLine( const MissionParameterPhaseLine& );            //!< Copy constructor
    MissionParameterPhaseLine& operator=( const MissionParameterPhaseLine& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< PointList > points_;
    std::string functions_;
    //@}
};

}
}

#endif // __MissionParameterPhaseLine_h_
