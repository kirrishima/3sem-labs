using System.Numerics;

namespace OOP_Lab03
{
    public partial class Array<T> where T : IComparable<T>, ISubtractionOperators<T, T, T>
    {
        protected T[] _array;
        public Array(int size)
        {
            _array = new T[size];
        }

        public T[] Data { get { return _array; } }

        public int Length { get { return _array.Length; } }

        public Array(T[] other)
        {
            _array = new T[other.Length];
            System.Array.Copy(other, _array, Length);
        }

        public Array(Array<T> other) : this(other._array)
        {

        }

        public Array(int[] arr)
        {
            _array = new T[arr.Length];
            System.Array.Copy(arr, _array, Length);
        }

        public override bool Equals(object? obj)
        {
            if (obj == null || this.GetType() != obj.GetType())
            {
                return false;
            }

            Array<T> other = (Array<T>)obj;

            if (this.Length != other.Length)
            {
                return false;
            }

            for (int i = 0; i < this.Length; i++)
            {
                if (this[i].CompareTo(other[i]) != 0)
                {
                    return false;
                }
            }

            return true;
        }

        public static Array<T> operator -(Array<T> obj, T value)
        {
            var tmp = new Array<T>(obj);

            for (int i = 0; i < obj.Length; i++)
            {
                obj[i] -= value;
            }

            return tmp;
        }

        public T this[int index]
        {
            get
            {
                return _array[index];
            }
            set
            {
                _array[index] = value;
            }
        }

        public static bool operator >(Array<T> obj, T value)
        {
            return obj._array.Contains(value);
        }

        public static bool operator <(Array<T> obj, T value)
        {
            return !(obj > value);
        }

        public static bool operator !=(Array<T> obj, Array<T> other)
        {
            return !obj.Equals(other);
        }

        public static bool operator ==(Array<T> obj, Array<T> other)
        {
            return obj.Equals(other);
        }

        public static Array<T> operator +(Array<T> obj1, Array<T> obj2)
        {
            Array<T> array = new(obj1.Length + obj2.Length);

            System.Array.Copy(obj1._array, array._array, obj1._array.Length);
            System.Array.Copy(obj2._array, 0, array._array, obj1.Length, obj2.Length);

            return array;
        }

        public override string ToString()
        {
            return $"[{string.Join(", ", _array)}]";
        }

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
