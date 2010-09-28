// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttribute_h_
#define __CrossingSiteAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace Common
{
    class ObjectAttributes;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  CrossingSiteAttribute
    @brief  CrossingSiteAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class CrossingSiteAttribute : public ObjectAttribute_ABC
                            , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CrossingSiteAttribute();
    explicit CrossingSiteAttribute( xml::xistream& xis );
    explicit CrossingSiteAttribute( const Common::MsgMissionParameter_Value& attributes );
    virtual ~CrossingSiteAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name ODB
    //@{
    virtual void WriteODB( xml::xostream& /*xos*/ ) const;
    //@}

    //! @name Network
    //@{
    virtual void SendFullState( Common::ObjectAttributes& asn ) const;
    virtual void SendUpdate( Common::ObjectAttributes& asn ) const;
    virtual void OnUpdate( const Common::MsgMissionParameter_Value& attribute );
    //@}

    //! @name Knowledge
    //@{
    virtual void Instanciate( DEC_Knowledge_Object& object ) const;
    //@}

    //! @name Accessors
    //@{
    MT_Float GetWidth() const;
    MT_Float GetDepth() const;
    MT_Float GetSpeed() const;
    bool IsBanksToFitOut() const;
    //@}

    //! @name Copy
    //@{
    CrossingSiteAttribute& operator=( const CrossingSiteAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    CrossingSiteAttribute( const CrossingSiteAttribute& from );
    //@}

private:
    //! @name
    //@{
    MT_Float rWidth_;
    MT_Float rDepth_;
    MT_Float rCurrentSpeed_;  // Vitesse du courant
    bool bBanksToFitOut_; // Berges a amenager
    //@}
};

BOOST_CLASS_EXPORT_KEY( CrossingSiteAttribute )

#endif // __CrossingSiteAttribute_h_
