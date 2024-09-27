namespace OOP_Lab03
{
    public partial class Array
    {
        public class Production
        {
            private int _id;
            private string _name;

            public int ID { get { return _id; } }
            public string Name { get { return _name; } set { _name = value; } }

            public Production(int id, string name)
            {
                _id = id;
                _name = name;
            }
        }

        public class Developer
        {
            private string _fullName;
            private int _id;
            private string _department;

            public Developer(string fullName, int id, string department)
            {
                _fullName = fullName;
                _id = id;
                _department = department;
            }
        }
    }
}
