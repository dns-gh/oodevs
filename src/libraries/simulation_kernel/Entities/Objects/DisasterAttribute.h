// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef DisasterAttribute_h
#define DisasterAttribute_h

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "MIL.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <memory>

namespace xml
{
    class xistream;
}

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class PropagationManager;
class ASCExtractor;

// =============================================================================
/** @class  DisasterAttribute
    @brief  Disaster attribute
*/
// Created: LGY 2012-10-05
// =============================================================================
class DisasterAttribute : public ObjectAttribute_ABC
                                , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DisasterAttribute();
    explicit DisasterAttribute( xml::xistream& xis );
    virtual ~DisasterAttribute();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void SendFullState( sword::ObjectAttributes& asn ) const;
    void UpdateLocalisation( MIL_Object_ABC& object, unsigned int time );

    DisasterAttribute& operator=( const DisasterAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< ASCExtractor > T_Extractor;
    //@}

private:
    //! @name Member data
    //@{
    std::string model_;
    std::string date_;
    std::auto_ptr< PropagationManager > pManager_;
    std::vector< T_Extractor > values_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DisasterAttribute )

#endif // DisasterAttribute_h
