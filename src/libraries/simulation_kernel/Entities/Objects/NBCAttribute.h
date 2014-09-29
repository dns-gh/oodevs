// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __NBCAttribute_h_
#define __NBCAttribute_h_

#include "ToxicAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeProxyRecon.h"
#include <boost/serialization/export.hpp>
#include <boost/serialization/split_member.hpp>

namespace xml
{
    class xistream;
}

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class MIL_NbcAgentType;

// =============================================================================
/** @class  NBCAttribute
    @brief  NBCAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class NBCAttribute : public ObjectAttribute_ABC
                   , public UpdatableAttribute_ABC
{
public:
    //! @name Types
    //@{
    typedef std::vector< const MIL_NbcAgentType* > T_NBCAgents;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             NBCAttribute();
    explicit NBCAttribute( xml::xistream& xis );
    explicit NBCAttribute( const sword::MissionParameter_Value& attributes );
    virtual ~NBCAttribute();
    //@}

    //! @name Knowledge
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Network update
    //@{
    virtual void SendFullState( sword::ObjectAttributes& asn ) const;
    virtual bool SendUpdate( sword::ObjectAttributes& asn ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Accessors
    //@{
    const T_NBCAgents& GetNBCAgents() const;
    unsigned int GetDangerLevel() const;
    bool IsContaminating() const;
    bool IsPoisonous() const;
    //@}

    //! @name Copy
    //@{
    NBCAttribute& operator=( const NBCAttribute& ); //!< Assignment operator
    bool Update( const NBCAttribute& rhs );
    bool UpdateCloudAttribute( const NBCAttribute& rhs );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadNBCAgent( xml::xistream& xis );
    bool ReadAgents( const std::string& strAgents );
    std::string WriteAgents() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Form
    {
        eLiquid,
        eGas
    };
    //@}

private:
    //! @name Member data
    //@{
    T_NBCAgents agents_;
    E_Form nForm_;
    unsigned int danger_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( NBCAttribute )

#endif // __NBCAttribute_h_
