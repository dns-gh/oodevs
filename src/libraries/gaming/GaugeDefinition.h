// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __GaugeDefinition_h_
#define __GaugeDefinition_h_

// =============================================================================
/** @class  GaugeDefinition
    @brief  GaugeDefinition
*/
// Created: SBO 2009-05-06
// =============================================================================
class GaugeDefinition
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit GaugeDefinition( xml::xistream& xis );
    virtual ~GaugeDefinition();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GaugeDefinition( const GaugeDefinition& );            //!< Copy constructor
    GaugeDefinition& operator=( const GaugeDefinition& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __GaugeDefinition_h_
