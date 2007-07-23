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
        private OrderFactory m_orderFactory = null;
        private IFeature     m_selectedFeature = null;
        private Order        m_order = null;

        public OrderManager()
        {
            Initialize();
        }

        private void Initialize()
        {
            IMxDocument doc = Tools.GetMxDocument();
            if (doc == null)
                return;
            m_orderFactory = new OrderFactory();
            ((IDocumentEvents_Event)doc).OnContextMenu += new IDocumentEvents_OnContextMenuEventHandler(OnContextMenu);
            //((IFeatureLayerSelectionEvents_Event)doc.FocusMap.get_Layer(0)).FeatureLayerSelectionChanged += new IFeatureLayerSelectionEvents_FeatureLayerSelectionChangedEventHandler(OnFeatureSelectionChanged);
        }

        private void OnFeatureSelectionChanged()
        {
            IMxDocument doc = Tools.GetMxDocument();
            if(doc == null)
                return;
            UpdateSelection(doc.FocusMap.FeatureSelection);
        }

        private void OnContextMenu(int x, int y, out bool handled)
        {
            OnFeatureSelectionChanged();

            handled = m_selectedFeature != null;
            if (!handled)
                return;
            IDocument doc = Tools.GetMxDocument() as IDocument;
            ICommandBar menu = doc.CommandBars.Create("Menu", esriCmdBarType.esriCmdBarTypeShortcutMenu);
            if (m_order == null)
                m_orderFactory.CreateMissionContextMenu(menu);
            else
                m_orderFactory.CreateMissionParameterContextMenu(menu);
            if (menu.Count > 0)
                menu.Popup(0, 0);
        }

        public void BindUI(IMissionForm form)
        {
            m_order = new Order(Tools.GetValue<int>(m_selectedFeature, "Public_OID"), form, m_orderFactory);
        }
        
        private void UpdateSelection(ISelection selection)
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
