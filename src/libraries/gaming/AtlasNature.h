// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AtlasNature_h_
#define __AtlasNature_h_

struct ASN1T_AtlasNature;

// =============================================================================
/** @class  AtlasNature
    @brief  AtlasNature
*/
// Created: SBO 2007-05-24
// =============================================================================
class AtlasNature
{

public:
    //! @name Constructors/Destructor
    //@{
             AtlasNature();
             AtlasNature( const QString& name, unsigned short value );
    virtual ~AtlasNature();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    unsigned short GetValue() const;
    bool IsSet( unsigned short value ) const;
    void CommitTo( ASN1T_AtlasNature& asn ) const;
    void Toggle( const AtlasNature& nature );
    //@}

private:
    //! @name Member data
    //@{
    QStringList name_;
    unsigned short value_;
    //@}
};

#endif // __AtlasNature_h_
