// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimeLimitedAttribute_h_
#define __TimeLimitedAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  TimeLimitedAttribute
    @brief  TimeLimitedAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class TimeLimitedAttribute : public ObjectAttribute_ABC
                           , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TimeLimitedAttribute();
    explicit TimeLimitedAttribute( xml::xistream& xis );
    explicit TimeLimitedAttribute( unsigned int nActivityTime );
    explicit TimeLimitedAttribute( const Common::MsgMissionParameter_Value& attributes );
    virtual ~TimeLimitedAttribute();
    //@}

    //! @name CheckPoints
    //@{    
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( Object& object ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const;    
    //@}

    //! @name Network updates
    //@{
    virtual void SendFullState( Common::MsgObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::MsgObjectAttributes& asn ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsTimeOver( unsigned int time ) const;
    //@}

    //! @name Copy
    //@{
    TimeLimitedAttribute& operator=( const TimeLimitedAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    TimeLimitedAttribute( const TimeLimitedAttribute& from );
    //@}

private:
    //! @name 
    //@{
    unsigned int nActivityTime_;
    mutable unsigned int nDeathTimeStep_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( TimeLimitedAttribute )

#endif
