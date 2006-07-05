// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaPrototype_h_
#define __RotaPrototype_h_

#include "ObjectPrototypeAttributes_ABC.h"
#include "Resolver.h"

class NBCAgent;
class RichLabel;

struct ASN1T_AttrObjectROTA;

// =============================================================================
/** @class  RotaPrototype
    @brief  RotaPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class RotaPrototype : public ObjectPrototypeAttributes_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RotaPrototype( QWidget* parent, const Resolver< NBCAgent >& resolver );
    virtual ~RotaPrototype();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Serialize( ASN1T_MagicActionCreateObject& msg );
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RotaPrototype( const RotaPrototype& );            //!< Copy constructor
    RotaPrototype& operator=( const RotaPrototype& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    unsigned GetAgentCount() const;
    //@}

private:
    //! @name Member data
    //@{
    RichLabel* nbcAgentsLabel_;
    QSpinBox* danger_;
    QListView* nbcAgents_;
    ASN1T_AttrObjectROTA* attr_;
    //@}
};

#endif // __RotaPrototype_h_
