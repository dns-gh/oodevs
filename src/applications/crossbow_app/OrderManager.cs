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
    public interface IController
    {
        void Register(IMissionObserver observer);        
        void Send(IMissionObserver observer);
        void Remove(IMissionObserver observer);
        void Update(IOrder element, IOrderParameter param);        
    }

    public class OrderManager : IController
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
        private IFeature m_targetFeature = null;
        private Order m_order = null;
        private Selection m_selection;
        private IMissionObserver m_observer;

        public OrderManager()
        {
            Initialize();
        }

        private void Initialize()
        {
            IMxDocument doc = Tools.GetMxDocument();
            if (doc == null)
                return;
            m_orderFactory = new OrderFactory(this);
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
            try
            {
                OnFeatureSelectionChanged();

                handled = m_targetFeature != null;
                if (!handled)
                    return;
                IDocument doc = Tools.GetMxDocument() as IDocument;
                ICommandBar menu = doc.CommandBars.Create("Menu", esriCmdBarType.esriCmdBarTypeShortcutMenu);
                if (m_order != null)
                    m_selection = new Selection(x, y);
                m_orderFactory.BuildMissionContextMenu(menu);
                if (menu.Count > 0)
                    menu.Popup(0, 0);
            }
            catch (Exception e)
            {
                System.Console.Write("OnContextMenu throws :" + e.Message);
                handled = false;
            }
        }

        public void Update(MultiItemSelectionMenu menu)
        {
            if (m_order == null)
                m_orderFactory.BuildMissionContextMenu(menu);
            else
                m_orderFactory.BuildMissionParameterContextMenu(menu, m_selection.X, m_selection.Y, m_order, m_targetFeature);
        }

        public void CreateOrder(string name)
        {
            m_orderFactory.CreateOrder(name, m_targetFeature, out m_order);            
        }

        public void SelectParameter(IOrderParameter param, string value)
        {
            if (m_order == null)
                return;
            m_order.SelectParameter(param, value);
            m_selection = null;
        }

        public void Register(IMissionObserver observer)
        {
            m_observer = observer;
            if (m_order != null)
                m_observer.NotifyCreated(m_order);
        }

        public void Update(IOrder element, IOrderParameter param)
        {
            if (m_observer != null)
                m_observer.NotifyUpdated(element, param);
        }        

        public void Remove(IMissionObserver observer)
        {
            if (m_observer != null)
                m_observer.NotifyDeleted(m_order);
            m_order = null;
        }
              
        private void UpdateSelection(ISelection selection)
        {
            IEnumFeature pEnumFeature = selection as IEnumFeature;
            pEnumFeature.Reset();
            m_targetFeature = pEnumFeature.Next();
        }

        public void Send(IMissionObserver observer)
        {
            ESRI.ArcGIS.ArcMapUI.IMxDocument mxDocument = Tools.GetMxDocument();
            IFeatureLayer                    pLayer = Tools.GetIFeatureLayerFromLayerName(mxDocument.ActiveView, "UnitForces");
            IFeatureWorkspace                workspace = Tools.RetrieveWorkspace(pLayer);
            if (workspace != null)
                m_order.Serialize(workspace);            
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
