using System;
using System.Collections.Generic;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Carto;
using ESRI.ArcGIS.SystemUI;

namespace crossbow
{
    public interface IParameterFactory
    {
        void CreateParameters(Order order);
        // void BuildMissionContextMenu(Order order);
    }

    public class OrderFactory : IParameterFactory
    {
        private class OrderParametersDefinition : Dictionary<string, string> {} //--> name, type

        private Dictionary<string, OrderParametersDefinition> m_model = new Dictionary<string, OrderParametersDefinition>();
        private FieldsProperty m_Fields = new FieldsProperty();
        private IController m_controller;

        public OrderFactory(IController controller)
        {
            m_controller = controller;
            Initialize();
        }

        private void Initialize()
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

        public void BuildMissionContextMenu(MultiItemSelectionMenu menu)
        {
            foreach (KeyValuePair<string, OrderParametersDefinition> elt in m_model)
                menu.Add(elt.Key.ToString());
        }

        public void BuildMissionParameterContextMenu(MultiItemSelectionMenu menu, int x, int y, IOrder order, IFeature feature)
        {
            order.OnContextMenu(menu, x, y, feature);
        }        

        public void CreateOrder(string name, IFeature feature, out Order order)
        {
            order = new Order(name, feature, this, m_controller);
            MissionOrderForm form = new MissionOrderForm(m_controller);            
        }

        public virtual void CreateParameters(Order order)
        {
            if (m_model.ContainsKey(order.Name))
                foreach (KeyValuePair<string, string> definition in m_model[order.Name])
                    order.RegisterParameter(new OrderParameter(definition.Key.ToString(), definition.Value.ToString()));
        }
    }
}
