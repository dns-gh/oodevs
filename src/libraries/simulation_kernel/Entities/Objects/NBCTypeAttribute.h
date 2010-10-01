// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __NBCTypeAttribute_h_
#define __NBCTypeAttribute_h_

#include "MIL.h"
#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "MIL_NBCType.h"
#include <boost/shared_ptr.hpp>
#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  NBCTypeAttribute
    @brief  NBCTypeAttribute
*/
// Created: RFT 2008-05-30
// =============================================================================
class NBCTypeAttribute : public ObjectAttribute_ABC
                       , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             NBCTypeAttribute();
    explicit NBCTypeAttribute( xml::xistream& xis );
    explicit NBCTypeAttribute( const Common::ObjectAttributes& asn );
    virtual ~NBCTypeAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Knowledge
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void Register( MIL_Object_ABC& object ) const;
    void SendFullState( Common::ObjectAttributes& asn ) const;
    void SendUpdate( Common::ObjectAttributes& asn ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void UpdateConcentration( int concentration );
    void ComputeAgentConcentrationEvolution( bool bHasASource );
    bool IsSource() const;
    void UpdateSourceLifeDuration( float time , float timeOfLastUpdate );
    //@}

    //! @name Get
    //@{
    int                   GetConcentration() const;
    const MIL_NBCType& GetAgentType() const;
    unsigned int       GetWidth() const;
    unsigned int       GetLength() const;
    unsigned int       GetPropagationAngle() const;
    int                GetSourceLifeDuration() const;
    //@}

    //! @name Operations
    //@{
    NBCTypeAttribute( const NBCTypeAttribute& ); //!< Copy operator
    NBCTypeAttribute& operator=( const NBCTypeAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    //Fire temperature
    const MIL_NBCType* pAgent_;
    int                concentration_;
    unsigned int       width_;
    unsigned int       length_;
    unsigned int       propagationAngle_;
    int                sourceLifeDuration_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( NBCTypeAttribute )

#endif // __NBCTypeAttribute_h_
