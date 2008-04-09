// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DisHeader_h_
#define __DisHeader_h_

namespace dis
{

// =============================================================================
/** @class  DisHeader
    @brief  DisHeader
*/
// Created: AGE 2008-03-10
// =============================================================================
class DisHeader
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DisHeader( unsigned char exercise );
            ~DisHeader();
    //@}

    //! @name Constructions
    //@{
    static DisHeader EntityStatePDU( unsigned long time, unsigned char exercise );
    static DisHeader DetonationPDU( unsigned long time, unsigned char exercise );
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << protocolVersion_ << exerciseIdentifier_ 
                << PDUType_ << protocolFamily_ << timeStamp_
                << length_ << padding_;
    }
    //@}

private:
    //! @name Member data
    //@{
    unsigned char  protocolVersion_;
    unsigned char  exerciseIdentifier_;
    unsigned char  PDUType_;
    unsigned char  protocolFamily_;
    unsigned long  timeStamp_;
    unsigned short length_;
    unsigned short padding_;
    //@}
};

}

#endif // __DisHeader_h_
