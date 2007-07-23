using System.Collections.Generic;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public class Order
    {
        private ParameterLimits m_limits = new ParameterLimits();
        private ParameterLimas m_limas = new ParameterLimas();
        private ParameterDirection m_direction = new ParameterDirection();
        private Dictionary<string, IOrderParameter> m_parameters = new Dictionary<string, IOrderParameter>();
        private IMissionForm m_handler = null;
        private int m_OID;

        public string Name
        {
            get
            {
                if (m_handler == null)
                    return "";
                return m_handler.OrderName;
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

        public Order(int id, IMissionForm handler, IParameterFactory factory)
        {
            OID = id;
            m_handler = handler;
            factory.CreateParameters(this);
        }

        public void RegisterParameter(IOrderParameter parameter)
        {
            m_handler.BindParameter(parameter);
            m_parameters.Add(parameter.Name, parameter);
        }

        public void OnContextMenu(ICommandBar menu, int x, int y, IFeature selected)
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

        public void Execute()
        {
            // $$$$ SBO 2007-07-23: TODO!
        }

    }
}
