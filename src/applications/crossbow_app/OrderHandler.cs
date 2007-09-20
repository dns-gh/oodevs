using System;
using System.Collections.Generic;
using System.Text;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;

namespace Crossbow
{    
    public class OrderHandler : IMultiItemContextMenuHandler
    {
        private class Selection
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

        private IFeature m_targetFeature;
        private Selection m_selection;
        private Order m_order;
        private OrderFactory m_orderFactory;

        public OrderHandler(WorkspaceConfiguration config)
        {
            m_orderFactory = new OrderFactory(config);
        }

        public IFeature TargetFeature
        {
            get
            {
                return m_targetFeature;
            }
        }

        public void Reset()
        {
            if (m_order == null)
                return;
            m_order.Dispose();
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

        public bool OnContextMenu(int x, int y)
        {
            if (!OnFeatureSelectionChanged(Tools.GetMxDocument(), x, y))
                return false;
            return m_orderFactory.OnContextMenu(x, y); // $$$$ SBO 2007-08-13: 
        }
              
        private void UpdateSelection(ISelection selection)
        {
            IEnumFeature pEnumFeature = selection as IEnumFeature;
            pEnumFeature.Reset();
            m_targetFeature = pEnumFeature.Next();
        }

        #region IMultiItemContextMenuHandler Members

        public void BuildContextMenu(MultiItemContextMenu menu)
        {
            if (m_order != null)
                m_order.OnContextMenu(menu, m_selection.X, m_selection.Y, m_targetFeature);
            else
                m_orderFactory.BuildMissionContextMenu(menu);
        }

        public void ActivateItem(string name, object value)
        {
            if (value == null)
                m_order = m_orderFactory.CreateOrder(name, this);
            else
            {
                if (m_order == null)
                    return;
                m_order.SelectParameter(value as IOrderParameter, name);
                m_selection = null;
            }
        }

        public string GetCurrentMessage()
        {
            if (m_order != null)
                return "Select parameter for " + m_order.Name + ".";
            else
                return "Select unit mission.";
        }

        #endregion
    }
}
