// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAtlasNature_h_
#define __ParamAtlasNature_h_

#include "Param_ABC.h"

// =============================================================================
/** @class  ParamAtlasNature
    @brief  ParamAtlasNature
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamAtlasNature : public QObject
                       , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamAtlasNature( QObject* parent, const QString& name );
    virtual ~ParamAtlasNature();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual void CommitTo( Action_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked( int id );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamAtlasNature( const ParamAtlasNature& );            //!< Copy constructor
    ParamAtlasNature& operator=( const ParamAtlasNature& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddField( const QString& name, unsigned char byte, unsigned char mask );
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::pair< QString, unsigned short > > T_AtlasFields;
    //@}

private:
    //! @name Member data
    //@{
    T_AtlasFields fields_;
    unsigned short bits_;
    //@}
};

#endif // __ParamAtlasNature_h_
