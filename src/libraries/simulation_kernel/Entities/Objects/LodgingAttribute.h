// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LodgingAttribute_h_
#define __LodgingAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyRecon.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FireAttribute
@brief  FireAttribute
*/
// Created: MMC 2011-05-04
// =============================================================================
class LodgingAttribute : public ObjectAttribute_ABC
                       , private UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LodgingAttribute();
    explicit LodgingAttribute( xml::xistream& xis );
    explicit LodgingAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~LodgingAttribute();
    //@}

    //! @name Knowledge
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    virtual void Register( MIL_Object_ABC& object ) const;
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Copy
    //@{
    LodgingAttribute& operator=( const LodgingAttribute& ); //!< Assignment operator
    bool Update( const LodgingAttribute& rhs );
    bool Update( unsigned int capacity );
    //@}

    //! @name Operations
    //@{
    void ManageResident( MIL_AgentPion& pion, MIL_AgentPion& transporter );
    void UnmanageResident( MIL_AgentPion& pion );
    //@}

private:
    //! @name Copy
    //@{
    LodgingAttribute( const LodgingAttribute& from );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_AgentPion* > T_ResidentsVector;
    typedef T_ResidentsVector::const_iterator CIT_ResidentsVector;
    typedef T_ResidentsVector::iterator IT_ResidentsVector;
    //@}

private:
    //! @name Helpers
    //@{
    unsigned int GetNbrManagedHumansResidents() const;
    //@}

private:
    //! @name
    //@{
    unsigned int capacity_;
    T_ResidentsVector Residents_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( LodgingAttribute )

#endif //__LodgingAttribute_h_
