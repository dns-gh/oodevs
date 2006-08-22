// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPart_ABC_h_
#define __PopulationPart_ABC_h_

#include "ASN_Types.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"

// =============================================================================
/** @class  PopulationPart_ABC
    @brief  Population part definition
*/
// Created: AGE 2006-02-20
// =============================================================================
class PopulationPart_ABC : public kernel::Entity_ABC
                         , public kernel::Updatable_ABC< ASN1T_MsgPopulationFluxUpdate >
                         , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationUpdate >
                         , public kernel::Drawable_ABC
                         , public kernel::Positions
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationPart_ABC();
    virtual ~PopulationPart_ABC();
    //@}

    //! @name Operations
    //@{
    virtual std::string  GetName() const = 0;
    virtual unsigned long GetId() const = 0;
    virtual unsigned int GetDeadHumans() const = 0;
    virtual unsigned int GetLivingHumans() const = 0;
    virtual unsigned int GetDensity() const = 0;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const = 0;

    virtual std::string  GetAttitude() const;

    virtual float GetHeight() const;

    virtual void Select( kernel::ActionController& controller ) const = 0;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const = 0;
    virtual void Activate( kernel::ActionController& controller ) const = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg );
    virtual void DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationPart_ABC( const PopulationPart_ABC& );            //!< Copy constructor
    PopulationPart_ABC& operator=( const PopulationPart_ABC& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    E_PopulationAttitude attitude_;
    //@}
};

#endif // __PopulationPart_ABC_h_
