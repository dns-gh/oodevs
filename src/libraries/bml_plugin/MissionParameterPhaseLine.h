// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterPhaseLine_h_
#define __MissionParameterPhaseLine_h_

#include "MissionParameter_ABC.h"
#include "protocol/Protocol.h"

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
    virtual void Serialize( sword::MissionParameter& parameter ) const;
    void Serialize( sword::PhaseLineOrder& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterPhaseLine( const MissionParameterPhaseLine& );            //!< Copy constructor
    MissionParameterPhaseLine& operator=( const MissionParameterPhaseLine& ); //!< Assignment operator
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
