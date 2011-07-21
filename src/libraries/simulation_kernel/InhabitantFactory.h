// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantFactory_h_
#define __InhabitantFactory_h_

#include "InhabitantFactory_ABC.h"

// =============================================================================
/** @class  InhabitantFactory
    @brief  InhabitantFactory
*/
// Created: SLG 2010-11-29
// =============================================================================
class InhabitantFactory : public InhabitantFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantFactory();
    virtual ~InhabitantFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Inhabitant& Create( xml::xistream& xis, MIL_Army_ABC& army );
    virtual void Finalize();
    //@}

    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const InhabitantFactory* factory, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, InhabitantFactory* factory, const unsigned int /*version*/ );
    //@}
};

BOOST_CLASS_EXPORT_KEY( InhabitantFactory )

template< typename Archive >
void save_construct_data( Archive& /*archive*/, const InhabitantFactory* /*factory*/, const unsigned int /*version*/ )
{
}

template< typename Archive >
void load_construct_data( Archive& /*archive*/, InhabitantFactory* factory, const unsigned int /*version*/ )
{
    ::new( factory )InhabitantFactory();
}

#endif // __InhabitantFactory_h_
