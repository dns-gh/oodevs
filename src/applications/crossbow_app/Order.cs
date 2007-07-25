using System.Collections.Generic;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public interface IOrder 
    {
        string Name { get; }
        string Target { get; }
        int OID { get; }
        void OnContextMenu(MultiItemSelectionMenu menu, int x, int y, IFeature selected);
    }

    public class Order : IOrder
    {
        private ParameterLimits m_limits = new ParameterLimits();
        private ParameterLimas m_limas = new ParameterLimas();
        private ParameterDirection m_direction = new ParameterDirection();
        private Dictionary<string, IOrderParameter> m_parameters = new Dictionary<string, IOrderParameter>();
        private IController m_controller;
        private int m_OID;
        private string m_name;
        private string m_target;

        public string Name
        {
            get
            {
                return m_name;
            }
        }
        public string Target
        {
            get
            {
                return m_target;
            }
        }
        public int OID
        {
            get
            {
                return m_OID;
            }
            set
            {
                m_OID = value;
            }
        }

        public Order(string name, IFeature feature, IParameterFactory factory, IController controller)
        {
            m_name = name;
            m_OID = Tools.GetValue<int>(feature, "Public_OID");
            m_target = Tools.GetValue<string>(feature, "Name");
            m_controller = controller;
            factory.CreateParameters(this);
        }

        public void RegisterParameter(IOrderParameter parameter)
        {
            m_parameters.Add(parameter.Name, parameter);
            m_controller.Update(this, parameter);
        }

        public void SelectParameter(IOrderParameter parameter, string value)
        {
            System.Console.Write("SelectParameter[ " + parameter.ToString() + "]:" + value);
            parameter.SetValue(value);
            m_controller.Update(this, parameter);
        }

        public void OnContextMenu(MultiItemSelectionMenu menu, int x, int y, IFeature selected)
        {            
            m_limits.OnContextMenu(menu, x, y, selected);
            m_limas.OnContextMenu(menu, x, y, selected);
            foreach (KeyValuePair<string, IOrderParameter> param in m_parameters)
                param.Value.OnContextMenu(menu, x, y, selected);
        }

        public void Serialize(IFeatureWorkspace featureWorkspace)
        {
            ITable table = featureWorkspace.OpenTable("OrdersParameters"); // $$$$ SBO 2007-07-20: keep it maybe...
            
            m_limits.Serialize(table, m_OID);
            m_limas.Serialize(table, m_OID);
            foreach (KeyValuePair<string, IOrderParameter> param in m_parameters)
                param.Value.Serialize(table, m_OID);
            table = null;
        }
    }
}
