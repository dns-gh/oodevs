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
    explicit MIL_LocationParameter( const sword::Location& );
    explicit MIL_LocationParameter( boost::shared_ptr< TER_Localisation > pLocalisation );
    virtual ~MIL_LocationParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToLocation( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
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
