// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __FormationFactory_h_
#define __FormationFactory_h_

#include "FormationFactory_ABC.h"

class AutomateFactory_ABC;

// =============================================================================
/** @class  FormationFactory
    @brief  FormationFactory
*/
// Created: MGD 2009-10-22
// =============================================================================
class FormationFactory : public FormationFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FormationFactory( AutomateFactory_ABC& automateFactory );
    virtual ~FormationFactory();
    //@}

    //! @name Operations
    //@{
    virtual MIL_Formation& Create( xml::xistream& xis, MIL_Army_ABC& army, MIL_Formation* parent = 0  );
    virtual MIL_Formation& Create( int level, const std::string& name, const std::string& logLevelStr, MIL_Army_ABC& army, MIL_Formation* parent = 0 );
    //@}
    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Operations
    //@{
    AutomateFactory_ABC& automateFactory_;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const FormationFactory* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, FormationFactory* role, const unsigned int /*version*/ );
    //@}
};

BOOST_CLASS_EXPORT_KEY( FormationFactory )

#endif // __FormationFactory_h_
