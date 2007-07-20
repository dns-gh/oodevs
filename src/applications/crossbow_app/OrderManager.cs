using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;

namespace crossbow
{
    public class OrderManager
    {
        private OrderFactory m_orderFactory;        
        private IFeature m_selectedFeature = null;
        private Order m_order = null;

        public OrderManager(OrderFactory orderFactory)
        {
            m_orderFactory = orderFactory;
        }

        public void OpenContextMenu(IFeatureSelection selection, int x, int y)
        {
            UpdateSelection(selection);

            if (m_order == null && selection.SelectionSet.Count == 1)
                m_orderFactory.OpenMissionContextMenu(x, y);
            else
                m_orderFactory.OpenMissionParameterContextMenu(x, y);
        }

        public void Register(MissionOrderForm handler)
        {
            m_order = new Order(handler);
            m_order.OID = Tools.GetValue<int>(m_selectedFeature, "Public_OID");            
            m_order.OnCreate(m_orderFactory);            
        }

        public object GetValue(IFeature feature, string name)
        {            
            return feature.get_Value(feature.Fields.FindField(name));
        }
         
        
        void UpdateSelection(IFeatureSelection selection)
        {
            IEnumFeature pEnumFeature = selection as IEnumFeature;
            pEnumFeature.Reset();
            m_selectedFeature = pEnumFeature.Next();
        }
        
        public string SelectionName()
        {            
            if (m_selectedFeature != null)
            {
                int iName = m_selectedFeature.Fields.FindField("Name");
                return m_selectedFeature.get_Value(iName) as string;
            }
            return "";
        }
                
        public void Execute()
        {
            IFeatureWorkspace workspace = null;
            if ( workspace != null )
                m_order.Serialize(workspace);
            Clear();
        }

        public void Cancel()
        {
            Clear();
        }

        private void Clear()
        {
            m_order = null;
        }
    }
}
