// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodAttribute_h_
#define __FloodAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "simulation_terrain/TER_Localisation.h"
#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  FloodAttribute
    @brief  FloodAttribute
*/
// Created: JSR 2010-12-15
// =============================================================================
class FloodAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodAttribute();
    explicit FloodAttribute( xml::xistream& xis, const TER_Localisation& objectLocation );
    explicit FloodAttribute( const sword::MissionParameter_Value& attributes, const TER_Localisation& objectLocation );
    virtual ~FloodAttribute();
    //@}

    //! @name Copy/Assignment
    //@{
    FloodAttribute& operator=( const FloodAttribute& ); //!< Assignment operator
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Network updates
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name Accessors
    //@{
    const TER_Localisation& GetLocalisation() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodAttribute( const FloodAttribute& );            //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    int depth_;
    int refDist_;
    TER_Localisation location_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( FloodAttribute )

#endif // __FloodAttribute_h_
