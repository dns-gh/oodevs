// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __InfrastructureParameters_h_
#define __InfrastructureParameters_h_

namespace gui 
{

// =============================================================================
/** @class  InfrastructureParameters
    @brief  InfrastructureParameters
*/
// Created: JSR 2010-06-28
// =============================================================================
class InfrastructureParameters
{

public:
    //! @name Constructors/Destructor
    //@{
    InfrastructureParameters() {};
    virtual ~InfrastructureParameters() {};
    //@}

public:
    //! @name Copy/Assignment
    //@{
    InfrastructureParameters( const InfrastructureParameters& );            //!< Copy constructor
    InfrastructureParameters& operator=( const InfrastructureParameters& ); //!< Assignment operator
    //@}

public:
    //! @name Member data
    //@{
    float structuralState_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: InfrastructureParameters constructor
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
inline
InfrastructureParameters::InfrastructureParameters( const InfrastructureParameters& from )
{
    structuralState_ = from.structuralState_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureParameters::operator=
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
inline
InfrastructureParameters& InfrastructureParameters::operator=( const InfrastructureParameters& from )
{
    structuralState_ = from.structuralState_;
    return *this;
}

}

#endif // __InfrastructureParameters_h_
