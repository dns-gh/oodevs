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
    public class OrderHandler
    {
        class Selection
        {
            int m_x;
            int m_y;
            public Selection(int x, int y) 
            {
                m_x = x;
                m_y = y;
            }
            public int X { get { return m_x; } }
            public int Y { get { return m_y; } }            
        };

        private OrderFactory m_orderFactory;        
        private IFeature m_targetFeature;
        private Selection m_selection;
        private Order m_order;

        public IFeature TargetFeature
        {
            get
            {
                return m_targetFeature;
            }
        }

        public OrderHandler(OrderFactory orderFactory)
        {
            m_orderFactory = orderFactory;            
        }

        public void Register(Order order)
        {
            m_order = order;            
        }

        public void Release()
        {
            m_order = null;
        }

        public bool OnFeatureSelectionChanged(IMxDocument mxDocument, int x, int y)
        {            
            if (mxDocument == null)            
                return false;
            UpdateSelection(mxDocument.FocusMap.FeatureSelection);
            if (m_order != null)
                m_selection = new Selection(x, y);
            return m_order != null || (m_targetFeature != null && m_targetFeature.Class.FindField("Public_OID") > 0); // m_order.NeedContextMenu()
        }
                
        public bool OnContextMenu(MultiItemSelectionMenu menu)
        {
            if (m_order != null)
                m_order.OnContextMenu(menu, m_selection.X, m_selection.Y, m_targetFeature);
            return m_order != null;
        }        

        public void SelectParameter(IOrderParameter param, string value)
        {
            if (m_order == null)
                return;
            m_order.SelectParameter(param, value);
            m_selection = null;
        }
              
        private void UpdateSelection(ISelection selection)
        {
            IEnumFeature pEnumFeature = selection as IEnumFeature;
            pEnumFeature.Reset();
            m_targetFeature = pEnumFeature.Next();
        }

        public string CurrentMessage
        {
            get
            {
                if (m_order != null)
                    return "Select parameter for " + m_order.Name + ".";
                else                    
                    return "Select unit mission.";
            }
        }        
    }
}
