// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationsEditor_h_
#define __DotationsEditor_h_

#include "clients_gui/ModalDialog.h"
#include "tools/Resolver.h"
#include "clients_kernel/ValueEditor.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class DotationType;
    class Entity_ABC;
}

class Dotation;
class DotationsItem;
class StaticModel;

// =============================================================================
/** @class  DotationsEditor
    @brief  DotationsEditor
*/
// Created: SBO 2006-11-10
// =============================================================================
class DotationsEditor : public ModalDialog
                      , public kernel::ValueEditor< DotationsItem* >
                      , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DotationsEditor( QDialog*& self, QWidget* parent, const ::StaticModel& staticModel );
    virtual ~DotationsEditor();
    //@}

    //! @name Operations
    //@{
    void SetCurrentItem( DotationsItem*& dotations, const kernel::Entity_ABC& current );
    virtual DotationsItem* GetValue();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClear();
    void OnAccept();
    void OnReject();
    void OnValueChanged( int row, int col );
    void OnLinkActivated( const QString& link );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::pair< double, double > > T_StockCapacities;
    typedef T_StockCapacities::const_iterator                  CIT_StockCapacities;

    enum E_InfosColumns { eWeightCurrent = 0, eWeightMax = 1, eVolumeCurrent = 2, eVolumeMax = 3 };

    class KeyPressSpinBox : public QSpinBox
    {
    public :
        void keyPressEvent( QKeyEvent *event ) { QSpinBox::keyPressEvent( event ); }
        void keyReleaseEvent( QKeyEvent *event ) { QSpinBox::keyReleaseEvent( event ); }
    };

    class KeyPressEditableTable : public Q3Table
    {
    public :
        KeyPressEditableTable( int numRows, int numCols, QWidget* parent = 0, const char* name = 0 ) : Q3Table( numRows, numCols, parent, name ) {}
        virtual ~KeyPressEditableTable() {}

    protected :
        virtual QWidget* beginEdit( int row, int col, bool replace )
        {
            QWidget* pWidget = Q3Table::beginEdit( row, col, replace );
            if( pWidget && col == 1 )
            {
                QSpinBox* pSpinBox = dynamic_cast< QSpinBox* >( pWidget );
                if( pSpinBox )
                    pSpinBox->selectAll();
            }
            return pWidget;
        }

        virtual KeyPressSpinBox* ManageLineEdit( QKeyEvent* event )
        {
            if( event && ( event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ) )
            {
                int row = currentRow(), col = currentColumn();
                QWidget* pWidget = this->cellWidget( row, col );
                if( pWidget && col == 1 )
                    if( QSpinBox* pSpinBox = dynamic_cast< QSpinBox* >( pWidget ) )
                        return static_cast< KeyPressSpinBox* >( pSpinBox );
            }
            return 0;
        }

        virtual void keyPressEvent( QKeyEvent* event )
        {
            KeyPressSpinBox* pSpinBox = ManageLineEdit( event );
            if( pSpinBox )
                pSpinBox->keyPressEvent( event );
            else
                Q3Table::keyPressEvent( event );
        }

        virtual void keyReleaseEvent( QKeyEvent* event )
        {
            KeyPressSpinBox* pSpinBox = ManageLineEdit( event );
            if( pSpinBox )
                pSpinBox->keyReleaseEvent( event );
            else
                Q3Table::keyReleaseEvent( event );
        }
    };
    //@}

    //! @name Helpers
    //@{
    void AddItem( const Dotation* dotation = 0 );
    void AddInfoItem( int row, E_InfosColumns currentCol, double currentValue, E_InfosColumns maxCol, double maxValue );
    void UpdateInfos();
    void FillMissingWithZero( T_StockCapacities& src, T_StockCapacities& dst );
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    const kernel::Entity_ABC* current_;
    DotationsItem** value_;
    KeyPressEditableTable* table_;
    Q3Table* infosTable_;
    QStringList types_;
    QDialog*& self_;
    QLabel* infosLabel_;
    static QColor warningColor_;
    //@}
};

#endif // __DotationsEditor_h_
