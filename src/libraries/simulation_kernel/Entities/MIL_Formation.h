// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Formation_h_
#define __MIL_Formation_h_

#include "MIL.h"
#include "tools/Resolver.h"
namespace xml
{
    class xostream;
    class xistream;
}

class MIL_Army_ABC;
class PHY_NatureLevel;

class FormationFactory_ABC;
class AutomateFactory_ABC;
// =============================================================================
/** @class  MIL_Formation
    @brief  MIL_Formation
*/
// Created: NLD 2006-10-11
// =============================================================================
class MIL_Formation : public tools::Resolver< MIL_Formation >
                    , public tools::Resolver< MIL_Automate >
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Formation(  xml::xistream& xis, MIL_Army_ABC& army, MIL_Formation* pParent, FormationFactory_ABC& formationFactory, AutomateFactory_ABC& automateFactory );
             MIL_Formation();
    virtual ~MIL_Formation();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()   
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

    void WriteODB             ( xml::xostream& xos ) const;
    void WriteLogisticLinksODB( xml::xostream& xos ) const;
    //@}

    //! @name Hierarchy
    //@{
    void RegisterAutomate   ( MIL_Automate& automate );
    void UnregisterAutomate ( MIL_Automate& automate );

    void RegisterFormation  ( MIL_Formation& formation );
    void UnregisterFormation( MIL_Formation& formation );

    //@}

    //! @name Accessors
    //@{
    MIL_Army_ABC& GetArmy() const;
    uint      GetID  () const;
    //@}

    //! @name Network
    //@{
    void SendCreation () const;
    void SendFullState() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_Formation( const MIL_Formation& );            //!< Copy constructor
    MIL_Formation& operator=( const MIL_Formation& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void InitializeFormation( xml::xistream& xis, FormationFactory_ABC& formationFactory );
    void InitializeAutomate( xml::xistream& xis, AutomateFactory_ABC& automateFactory );
    //@}

private:
    //! @name Attributes
    //@{
          uint              nID_;
          MIL_Army_ABC*     pArmy_;
          MIL_Formation*    pParent_;
    const PHY_NatureLevel*  pLevel_;
          std::string       strName_;
    //@}
};

#endif // __MIL_Formation_h_
