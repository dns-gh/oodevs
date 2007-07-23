using System;
using System.Collections.Generic;
using ESRI.ArcGIS.Framework;
using ESRI.ArcGIS.esriSystem;

namespace crossbow
{
    public interface IParameterFactory
    {
        void CreateParameters(Order order);
    }

    public class OrderFactory : IParameterFactory
    {
        private class OrderParametersDefinition : Dictionary<string, string> {} //--> name, type

        private Dictionary<string, OrderParametersDefinition> m_model = new Dictionary<string, OrderParametersDefinition>();
        private FieldsProperty m_Fields = new FieldsProperty();

        public OrderFactory()
        {
            Initialize();
        }

        private void Initialize()
        {
            m_model.Add("Armor - To attack", new OrderParametersDefinition());
            {
                OrderParametersDefinition param = new OrderParametersDefinition();
                param.Add("Deployment area", "shape");
                m_model.Add("Armor - To hold", param);
            }
            {
                OrderParametersDefinition param = new OrderParametersDefinition();
                param.Add("Unit", "automat");
                m_model.Add("Armor - To support (dynamic)", param);
            }
            {
                OrderParametersDefinition param = new OrderParametersDefinition();
                param.Add("Position to deny", "point");
                param.Add("Obstacle erection", "bool");
                m_model.Add("Infantery - To deny", param);
            }
            m_model.Add("Infantery - To reconnoiter", new OrderParametersDefinition());
        }

        public void CreateMissionContextMenu(ICommandBar menu)
        {
            BuildMissionContextMenu(menu);
        }

        public void CreateMissionParameterContextMenu(ICommandBar menu)
        {
            // $$$$ SBO 2007-07-23: TODO!
        }

        static private UID getTypeUid(Type type)
        {
            UID uid = new UIDClass();
            uid.Value = "{" + type.GUID.ToString() + "}";
            return uid;
        }

        void BuildMissionContextMenu(ICommandBar menu)
        {
            object Missing = Type.Missing;
            ICommandBar subMenu = menu.CreateMenu("Missions", ref Missing);

            UID menuUID = new UIDClass();            
            menuUID.Value = "crossbow.MissionOrder_ArmorAttack";
            subMenu.Add(menuUID, ref Missing);            
            menuUID.Value = "crossbow.MissionOrder_ArmorSupport";
            subMenu.Add(menuUID, ref Missing);
            menuUID.Value = "crossbow.MissionOrder_ArmorHold";
            subMenu.Add(menuUID, ref Missing);
            menuUID.Value = "crossbow.MissionOrder_InfanteryReconnoiter";
            subMenu.Add(menuUID, ref Missing);
            menuUID.Value = "crossbow.MissionOrder_InfanteryDeny";
            subMenu.Add(menuUID, ref Missing);
        }

        public virtual void CreateParameters(Order order)
        {
            if (m_model.ContainsKey(order.Name))
                foreach (KeyValuePair<string, string> definition in m_model[order.Name])
                    order.RegisterParameter(new OrderParameter(definition.Key.ToString(), definition.Value.ToString()));
        }
    }
}
