// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Order_ABC_h_
#define __Order_ABC_h_

struct ASN1T_MissionParameters;

namespace dispatcher
{
    class MissionParameter_ABC;
    class Model;

// =============================================================================
/** @class  Order_ABC
    @brief  Order_ABC
*/
// Created: NLD 2007-04-20
// =============================================================================
class Order_ABC
{
protected:
    //! @name Constructors/Destructor
    //@{
             Order_ABC( Model& model, unsigned int missionID, const ASN1T_MissionParameters& parameters );
    virtual ~Order_ABC();
    //@}

    //! @name Operations
    //@{
    void Send     ( ASN1T_MissionParameters& asn ) const;
    void AsnDelete( ASN1T_MissionParameters& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Order_ABC( const Order_ABC& );            //!< Copy constructor
    Order_ABC& operator=( const Order_ABC& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    void InitializeParameters( const ASN1T_MissionParameters& parameters );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MissionParameter_ABC* > T_Parameters;
    typedef T_Parameters::const_iterator         CIT_Parameters;
    //@}

protected:
    //! @name Member data
    //@{
    Model& model_;
    unsigned int missionID_;
    T_Parameters parameters_;
    //@}
};

}

#endif // __Order_ABC_h_
