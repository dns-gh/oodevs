using System;
using System.Collections.Generic;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.ArcMapUI;

namespace crossbow
{
    public interface IParameterFactory
    {
        void CreateParameters(IOrder order);        
    }

    public class OrderFactory : IParameterFactory
    {
        private class OrderParametersDefinition : Dictionary<string, string> {} //--> name, type

        private Dictionary<string, OrderParametersDefinition> m_model = new Dictionary<string, OrderParametersDefinition>();
        private FieldsProperty m_Fields = new FieldsProperty();
        private OrderHandler m_handler;

        public OrderFactory()
        {
            m_handler = new OrderHandler(this);
            
            InitializeModel();            
        }

        public OrderHandler OrderHandler
        {
            get
            {
                return m_handler;
            }
        }

        private void InitializeModel()
        {
            m_model.Add("Armor - To attack", new OrderParametersDefinition());

            {
                OrderParametersDefinition param = new OrderParametersDefinition();
                param.Add("Deployment area", "Polygon");
                m_model.Add("Armor - To hold", param);
            }
            {
                OrderParametersDefinition param = new OrderParametersDefinition();
                param.Add("Unit to support", "Agent");
                m_model.Add("Armor - To support (dynamic)", param);
            }
            {
                OrderParametersDefinition param = new OrderParametersDefinition();
                param.Add("Position to deny", "Point");
                param.Add("Obstacle erection", "Bool");
                m_model.Add("Infantry - To deny", param);
            }
            m_model.Add("Infantry - To reconnoiter", new OrderParametersDefinition());
        }

        public void BuildMissionContextMenu(ICommandBar menu)
        {
            object Missing = Type.Missing;

            UID uid = new UIDClass();
            uid.Value = "crossbow.MultiItemSelectionMenu";
            menu.Add(uid, ref Missing);
        }

        public void OnContextMenu(int x, int y, out bool handled)
        {
            try
            {
                IMxDocument mxDocument = Tools.GetMxDocument();

                handled = m_handler.OnFeatureSelectionChanged(mxDocument, x, y);
                if (!handled)
                    return;
                ICommandBar menu = ((IDocument)mxDocument).CommandBars.Create("Menu", esriCmdBarType.esriCmdBarTypeShortcutMenu);
                BuildMissionContextMenu(menu);
                if (menu.Count > 0)
                    menu.Popup(0, 0);
            }
            catch (Exception e)
            {
                System.Console.Write("OnContextMenu throws :" + e.Message);
                handled = false;
            }
        }

        public void OnContextMenu(MultiItemSelectionMenu menu)
        {
            if (!m_handler.OnContextMenu(menu))
                BuildMissionContextMenu(menu);
        }

        public void BuildMissionContextMenu(MultiItemSelectionMenu menu)
        {
            foreach (KeyValuePair<string, OrderParametersDefinition> elt in m_model)
                menu.Add(elt.Key.ToString());
        }
        
        public void CreateOrder(string name)
        {
            m_handler.Register(new Order(name, m_handler, this));
        }

        public virtual void CreateParameters(IOrder order)
        {
            if (m_model.ContainsKey(order.Name))
                foreach (KeyValuePair<string, string> definition in m_model[order.Name])
                    order.RegisterParameter(new OrderParameter(definition.Key.ToString(), definition.Value.ToString()));
        }
    }
}
