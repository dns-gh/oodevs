// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class MIL_AutomateLOG;

// =============================================================================
/** @class  FireAttribute
    @brief  FireAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class LogisticAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticAttribute();
    explicit LogisticAttribute( xml::xistream& xis );
    explicit LogisticAttribute( const Common::MsgMissionParameter_Value& attributes );
    virtual ~LogisticAttribute();
    //@}

    //! @name Knowledge
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( Common::MsgObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::MsgObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name virtual
    //@{
    MIL_AutomateLOG& GetTC2() const;
    //@}

    //! @name Copy
    //@{
    LogisticAttribute& operator=( const LogisticAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    LogisticAttribute( const LogisticAttribute& from );
    //@}

private:
    //! @name
    //@{
    MIL_AutomateLOG* pTC2_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( LogisticAttribute )

#endif //__LogisticAttribute_h_
