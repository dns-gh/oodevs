using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.Geodatabase;

namespace crossbow
{
    class FieldsProperty
    {        
        public enum EnumFields { eOID, eParentOID, eName, eSymbol, eFormationOID };
        
        private int[] m_formationFields;
        private int[] m_tacticalElementFields;

        public FieldsProperty()
        {
            m_formationFields = new int[4];
            m_tacticalElementFields = new int[5];
        }

        ~FieldsProperty()
        {
        }

        #region accessors
        public int[] Formation
        {
            get                
            {
                return m_formationFields;
            }            
        }

        public int[] Element
        {
            get
            {
                return m_tacticalElementFields;
            }
        }
        #endregion

        public void SetupFormationFields(ITable pTable)
        {
            m_formationFields[(int)EnumFields.eSymbol] = pTable.FindField("Symbol_ID");
            m_formationFields[(int)EnumFields.eOID] = pTable.FindField("Public_OID");
            m_formationFields[(int)EnumFields.eParentOID] = pTable.FindField("Parent_OID");
            m_formationFields[(int)EnumFields.eName] = pTable.FindField("Name");
        }

        public void SetupTacticalElementFields(IFeatureClass pFeatureClass)
        {
            m_tacticalElementFields[(int)EnumFields.eSymbol] = pFeatureClass.FindField("Symbol_ID");
            m_tacticalElementFields[(int)EnumFields.eOID] = pFeatureClass.FindField("Public_OID");
            m_tacticalElementFields[(int)EnumFields.eParentOID] = pFeatureClass.FindField("Parent_OID");
            m_tacticalElementFields[(int)EnumFields.eName] = pFeatureClass.FindField("Name");
            m_tacticalElementFields[(int)EnumFields.eFormationOID] = pFeatureClass.FindField("Formation_OID");
        }
    }
}
