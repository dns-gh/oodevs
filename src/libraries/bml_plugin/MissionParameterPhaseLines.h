// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterPhaseLines_h_
#define __MissionParameterPhaseLines_h_

#include "MissionParameter_ABC.h"

namespace xml
{
    class xistream;
}

namespace bml
{
    class MissionParameterPhaseLine;

// =============================================================================
/** @class  MissionParameterPhaseLines
    @brief  MissionParameterPhaseLines
*/
// Created: SBO 2008-05-22
// =============================================================================
class MissionParameterPhaseLines : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionParameterPhaseLines( const kernel::OrderParameter& type );
    virtual ~MissionParameterPhaseLines();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ASN1T_MissionParameter& parameter ) const;
    virtual void Clean( ASN1T_MissionParameter& parameter ) const;
    void AddLine( MissionParameterPhaseLine& line );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterPhaseLines( const MissionParameterPhaseLines& );            //!< Copy constructor
    MissionParameterPhaseLines& operator=( const MissionParameterPhaseLines& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< MissionParameterPhaseLine* > T_PhaseLines;
    //@}

private:
    //! @name Member data
    //@{
    T_PhaseLines phaseLines_;
    //@}
};

}

#endif // __MissionParameterPhaseLines_h_
