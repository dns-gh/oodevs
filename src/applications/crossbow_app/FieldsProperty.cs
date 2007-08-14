using System;
using ESRI.ArcGIS.Geodatabase;

namespace Crossbow
{
    public class FieldsProperty
    {        
        public enum EnumFields { eOID, eParentOID, eName, eSymbol, eFormationOID };
                
        private int[] m_formationFields;
        private int[] m_tacticalElementFields;

        public FieldsProperty()
        {
            m_formationFields = new int[4];
            m_tacticalElementFields = new int[5];
        }

        #region Accessors
        public int GetFormationFieldIndex(EnumFields field)
        {
            return m_formationFields[(int)field];
        }

        public int GetElementFieldIndex(EnumFields field)
        {
            return m_tacticalElementFields[(int)field];
        }
        #endregion

        public void SetupFormationFields(ITable pTable)
        {
            if (pTable == null)
                return;
            m_formationFields[(int)EnumFields.eSymbol] = pTable.FindField("Symbol_ID");
            m_formationFields[(int)EnumFields.eOID] = pTable.FindField("Public_OID");
            m_formationFields[(int)EnumFields.eParentOID] = pTable.FindField("Parent_OID");
            m_formationFields[(int)EnumFields.eName] = pTable.FindField("Name");
        }

        public void SetupTacticalElementFields(IFeatureClass pFeatureClass)
        {
            if (pFeatureClass == null)
                return;
            m_tacticalElementFields[(int)EnumFields.eSymbol] = pFeatureClass.FindField("Symbol_ID");
            m_tacticalElementFields[(int)EnumFields.eOID] = pFeatureClass.FindField("Public_OID");
            m_tacticalElementFields[(int)EnumFields.eParentOID] = pFeatureClass.FindField("Parent_OID");
            m_tacticalElementFields[(int)EnumFields.eName] = pFeatureClass.FindField("Name");
            m_tacticalElementFields[(int)EnumFields.eFormationOID] = pFeatureClass.FindField("Formation_OID");
        }
    }
}
