// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#include "clients_kernel/Types.h"
#include "ASN_Types.h"

#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/OptionalValue.h"

class PopulationPart_ABC;
class PopulationConcentration;
class PopulationFlow;
class Team_ABC;
class Controller;
class PopulationType;
class CoordinateConverter_ABC;
class Displayer_ABC;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Population : public Population_ABC
                 , public Updatable_ABC< ASN1T_MsgPopulationFluxCreation > // $$$$ AGE 2006-03-13: dégager dans des extensions ?
                 , public Updatable_ABC< ASN1T_MsgPopulationFluxUpdate >
                 , public Updatable_ABC< ASN1T_MsgPopulationFluxDestruction >
                 , public Updatable_ABC< ASN1T_MsgPopulationUpdate >       
                 , public Updatable_ABC< ASN1T_MsgPopulationConcentrationCreation >
                 , public Updatable_ABC< ASN1T_MsgPopulationConcentrationUpdate > 
                 , public Updatable_ABC< ASN1T_MsgPopulationConcentrationDestruction >
                 , public Drawable_ABC
                 , public Positions
{
public:
    //! @name Constructor/Destructor
    //@{
             Population( const ASN1T_MsgPopulationCreation& asnMsg, Controller& controller, const CoordinateConverter_ABC& converter,
                         const Resolver_ABC< Team_ABC >& teamResolver, const Resolver_ABC< PopulationType >& typeResolver );
    virtual ~Population();
    //@}

    //! @name Operations
    //@{
    void DisplayInTooltip( Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
    unsigned int GetLivingHumans() const;
    unsigned int GetDeadHumans() const;

    virtual bool IsInTeam( const Team_ABC& team ) const;
    virtual geometry::Point2f GetPosition() const;
    virtual float             GetHeight() const;
    virtual bool IsAt( const geometry::Point2f& pos, float precision = 100.f ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    //@}

    //! @name Accessors
    //@{
    virtual const Team_ABC& GetTeam() const;
    virtual unsigned long GetId() const;
    virtual std::string   GetName() const;
    const PopulationType& GetType() const;
    //@}
    
private:
    //! @name Copy / Assignment
    //@{
    Population( const Population& );
    Population& operator=( const Population& );
    //@}

    //! @name Helpers
    //@{
    unsigned int ComputeLivingHumans() const;
    unsigned int ComputeDeadHumans() const;

    void DoUpdate( const ASN1T_MsgPopulationFluxCreation&             message );
    void DoUpdate( const ASN1T_MsgPopulationFluxUpdate&               message );
    void DoUpdate( const ASN1T_MsgPopulationFluxDestruction&          message );
    void DoUpdate( const ASN1T_MsgPopulationUpdate&                   message );
    void DoUpdate( const ASN1T_MsgPopulationConcentrationCreation&    message );
	void DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate&      message );
	void DoUpdate( const ASN1T_MsgPopulationConcentrationDestruction& message );

    void ComputeCenter();
    //@}

private:
    //! @name Member data
    //@{
    Controller&                controller_;
    const CoordinateConverter_ABC& converter_;
	unsigned long              nPopulationID_;
	std::string                strName_;
    const PopulationType&      type_;
	Team_ABC&                  team_;

    geometry::Point2f          center_;
    OptionalValue< int >       nDomination_;
    //@}

private:
    static unsigned long nMaxId_;
};

#endif // __Population_h_
