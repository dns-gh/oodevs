// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyRecon.h"
#include <boost/serialization/export.hpp>

namespace logistic
{
    class ObjectLogisticHierarchy;
    class LogisticHierarchy_ABC;
}

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
                        , private UpdatableAttribute_ABC
{
public:
    typedef DEC_Knowledge_ObjectAttributeProxyRecon< LogisticAttribute > T_KnowledgeProxyType;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticAttribute();
    explicit LogisticAttribute( xml::xistream& xis );
    explicit LogisticAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~LogisticAttribute();
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
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Finalize();
    const logistic::LogisticHierarchy_ABC& GetLogisticHierarchy() const;
    //@}

    //! @name Copy
    //@{
    LogisticAttribute& operator=( const LogisticAttribute& ); //!< Assignment operator
    bool Update( const LogisticAttribute& rhs );
    //@}

private:
    //! @name Copy
    //@{
    LogisticAttribute( const LogisticAttribute& from );
    //@}

private:
    //! @name
    //@{
    std::auto_ptr< logistic::ObjectLogisticHierarchy > pLogisticHierarchy_;
    unsigned int idFromXML_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( LogisticAttribute )

#endif //__LogisticAttribute_h_
