// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLimits_h_
#define __ParamLimits_h_

#include "EntityParameter.h"
#include "Param_ABC.h"
class Limit;

// =============================================================================
/** @class  ParamLimits
    @brief  ParamLimits
*/
// Created: AGE 2006-03-24
// =============================================================================
class ParamLimits : public Param_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamLimits( QWidget* pParent, ASN1T_OID& id1, ASN1T_OID& id2, 
                 const QString& label1, const QString& label2, 
                 const QString& menu1, const QString& menu2 );
    virtual ~ParamLimits();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void SetOptional( bool );
    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& extent, const GlTools_ABC& tools ) const;
    virtual void RegisterIn( ActionController& controller );
    virtual bool CheckValidity();
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamLimits( const ParamLimits& );            //!< Copy constructor
    ParamLimits& operator=( const ParamLimits& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    EntityParameter< Limit >* limit1_;
    EntityParameter< Limit >* limit2_;
    //@}
};

#endif // __ParamLimits_h_
