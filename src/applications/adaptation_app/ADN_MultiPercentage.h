// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MultiPercentage_h_
#define __ADN_MultiPercentage_h_

#include "ADN_EditLine.h"
#include "ADN_GuiBuilder.h"

class ADN_GuiBuilder;
class ADN_Connector_ABC;

// =============================================================================
/** @class  ADN_MultiPercentage
    @brief  ADN_MultiPercentage
*/
// Created: ABR 2012-11-19
// =============================================================================
class ADN_BaseMultiPercentage : public QObject
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_BaseMultiPercentage( QObject* parent ) : QObject( parent ) {}
    //@}

protected slots:
    //! @name Slots
    //@{
    virtual void OnPercentageChanged() {}
    virtual void OnEditingFinished() {}
    //@}

protected:
    QString GetPercentLabel() { return tr( "%" ); }
};

// =============================================================================
/** @class  ADN_MultiPercentage
    @brief  ADN_MultiPercentage
*/
// Created: ABR 2011-03-18
// =============================================================================
template< typename EditLine >
class ADN_MultiPercentage : public ADN_BaseMultiPercentage
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_MultiPercentage( Q3GroupBox* pParent, ADN_GuiBuilder& builder, const QString& objectName )
        : ADN_BaseMultiPercentage( pParent )
        , parent_ ( pParent )
        , builder_( builder )
        , warning_( 0 )
    {
        setObjectName( objectName );
    }
    virtual ~ADN_MultiPercentage()
    {
        lines_.clear();
    }
    //@}

    //! @name Operations
    //@{
    void AddLine( const char* szName, ADN_Connector_ABC*& itemConnector, const char* lineObjectName )
    {
        EditLine* pLine = builder_.AddField< EditLine >( parent_, lineObjectName, szName, itemConnector, GetPercentLabel(), ePercentage );
        connect( pLine, SIGNAL( textEdited( const QString& ) ), this, SLOT( OnPercentageChanged() ) );
        connect( pLine, SIGNAL( StartEditing() ), this, SLOT( OnPercentageChanged() ) );
        connect( pLine, SIGNAL( editingFinished() ), this, SLOT( OnEditingFinished() ) );
        lines_.push_back( pLine );
    }
    void AddWarning()
    {
        parent_->addSpace( 0 );
        warning_ = new QLabel( parent_ );
        warning_->setObjectName( objectName() + "_warning" );
        parent_->addSpace( 0 );
    }
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnPercentageChanged();
    virtual void OnEditingFinished()
    {
        for( auto it = lines_.begin(); it != lines_.end(); ++it )
            ( *it )->GetValidator().setTop( 100 );
    }
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< EditLine* > T_EditLines;
    //@}

private:
    //! @name Member data
    //@{
    Q3GroupBox*              parent_;
    ADN_GuiBuilder&          builder_;
    std::vector< EditLine* > lines_;
    T_ConnectorVector        connectors_;
    QLabel*                  warning_;
    //@}
};

typedef ADN_MultiPercentage< ADN_EditLine_Int >    ADN_MultiPercentage_Int;
typedef ADN_MultiPercentage< ADN_EditLine_Double > ADN_MultiPercentage_Double;

#endif // __ADN_MultiPercentage_h_
