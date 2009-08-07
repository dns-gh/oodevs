// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LocationParameter_h_
#define __MIL_LocationParameter_h_

#include "MIL_BaseParameter.h"
#include "simulation_terrain/TER_Localisation.h"

// =============================================================================
/** @class  MIL_LocationParameter
    @brief  MIL_LocationParameter
*/
// Created: LDC 2009-05-25
// =============================================================================
class MIL_LocationParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_LocationParameter( const ASN1T_Location& );
    explicit MIL_LocationParameter( boost::shared_ptr< TER_Localisation > pLocalisation );
    virtual ~MIL_LocationParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToLocation( ASN1T_Location& ) const;
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPolygon( ASN1T_Polygon& ) const;
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_LocationParameter( const MIL_LocationParameter& );            //!< Copy constructor
    MIL_LocationParameter& operator=( const MIL_LocationParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< TER_Localisation > pLocalisation_;
    //@}
};

#endif // __MIL_LocationParameter_h_
