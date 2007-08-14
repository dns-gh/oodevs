using System;
using System.Collections.Generic;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.SystemUI;
using ESRI.ArcGIS.ArcMapUI;

namespace Crossbow
{
    public interface IParameterFactory
    {
        void CreateParameters(IOrder order);        
    }

    public class OrderFactory : IParameterFactory
    {
        private class OrderParametersDefinition : Dictionary<string, string> {} //--> name, type

        private Dictionary<string, OrderParametersDefinition> m_model = new Dictionary<string, OrderParametersDefinition>();

        public OrderFactory()
        {
            InitializeModel();
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

        public static void BuildMissionContextMenu(ICommandBar menu)
        {
            if (menu == null)
                return;

            UID uid = new UIDClass();
            uid.Value = "Crossbow.MultiItemSelectionMenu";
            object Missing = Type.Missing;
            menu.Add(uid, ref Missing);
        }

        public bool OnContextMenu(int x, int y)
        {
            try
            {
                ICommandBar menu = ((IDocument)Tools.GetMxDocument()).CommandBars.Create("Menu", esriCmdBarType.esriCmdBarTypeShortcutMenu);
                BuildMissionContextMenu(menu);
                if (menu.Count > 0)
                    menu.Popup(0, 0);
                return true;
            }
            catch (Exception e)
            {
                System.Console.Write("OnContextMenu throws :" + e.Message);
                return false;
            }
        }

        public void BuildMissionContextMenu(MultiItemSelectionMenu menu)
        {
            foreach (KeyValuePair<string, OrderParametersDefinition> elt in m_model)
                menu.Add(elt.Key.ToString());
        }
        
        public Order CreateOrder(string name, OrderHandler handler)
        {
            return new Order(name, handler, this);
        }

        public virtual void CreateParameters(IOrder order)
        {
            if (m_model.ContainsKey(order.Name))
                foreach (KeyValuePair<string, string> definition in m_model[order.Name])
                    order.RegisterParameter(new OrderParameter(definition.Key.ToString(), definition.Value.ToString()));
        }
    }
}
