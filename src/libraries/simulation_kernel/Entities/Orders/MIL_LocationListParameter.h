// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LocationListParameter_h_
#define __MIL_LocationListParameter_h_

#include "MIL_BaseParameter.h"
#include "simulation_terrain/TER_Localisation.h"

// =============================================================================
/** @class  MIL_LocationListParameter
    @brief  MIL_LocationListParameter
*/
// Created: LDC 2009-05-26
// =============================================================================
class MIL_LocationListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_LocationListParameter( const ASN1T_LocationList& );
    explicit MIL_LocationListParameter( const std::vector< boost::shared_ptr< TER_Localisation > >& locationList );
    virtual ~MIL_LocationListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToLocationList( ASN1T_LocationList& ) const;
    virtual bool ToLocationList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_LocationListParameter( const MIL_LocationListParameter& );            //!< Copy constructor
    MIL_LocationListParameter& operator=( const MIL_LocationListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< TER_Localisation > > locationList_;
    //@}
};

#endif // __MIL_LocationListParameter_h_
